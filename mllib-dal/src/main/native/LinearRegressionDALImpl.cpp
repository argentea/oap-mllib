/*******************************************************************************
 * Copyright 2020 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include <chrono>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <vector>

#ifdef CPU_GPU_PROFILE
#include "GPU.h"
#ifndef ONEDAL_DATA_PARALLEL
#define ONEDAL_DATA_PARALLEL
#endif
#include "Communicator.hpp"
#include "OutputHelpers.hpp"
#include "oneapi/dal/algo/linear_regression.hpp"
#include "oneapi/dal/algo/linear_regression/train_types.hpp"
#include "oneapi/dal/table/homogen.hpp"
#endif

#include "OneCCL.h"
#include "com_intel_oap_mllib_regression_LinearRegressionDALImpl.h"
#include "service.h"

using namespace std;
#ifdef CPU_GPU_PROFILE
using namespace oneapi::dal;
#else
using namespace daal;
using namespace daal::algorithms;
using namespace daal::services;
#endif

typedef double algorithmFPType; /* Algorithm floating-point type */

#ifdef CPU_ONLY_PROFILE
static NumericTablePtr linear_regression_compute(int rankId,
                                                 ccl::communicator &comm,
                                                 const NumericTablePtr &pData,
                                                 const NumericTablePtr &pLabel,
                                                 size_t nBlocks) {
    using daal::byte;

    linear_regression::training::Distributed<step1Local> localAlgorithm;

    /* Pass a training data set and dependent values to the algorithm */
    localAlgorithm.input.set(linear_regression::training::data, pData);
    localAlgorithm.input.set(linear_regression::training::dependentVariables,
                             pLabel);

    /* Train the multiple linear regression model on local nodes */
    localAlgorithm.compute();

    /* Serialize partial results required by step 2 */
    services::SharedPtr<byte> serializedData;
    InputDataArchive dataArch;
    localAlgorithm.getPartialResult()->serialize(dataArch);
    size_t perNodeArchLength = dataArch.getSizeOfArchive();

    serializedData =
        services::SharedPtr<byte>(new byte[perNodeArchLength * nBlocks]);

    byte *nodeResults = new byte[perNodeArchLength];
    dataArch.copyArchiveToArray(nodeResults, perNodeArchLength);
    std::vector<size_t> aReceiveCount(comm.size(),
                                      perNodeArchLength); // 4 x "14016"

    /* Transfer partial results to step 2 on the root node */
    ccl::gather((int8_t *)nodeResults, perNodeArchLength,
                (int8_t *)(serializedData.get()), perNodeArchLength, comm)
        .wait();

    delete[] nodeResults;

    NumericTablePtr resultTable;
    if (rankId == ccl_root) {
        /* Create an algorithm object to build the final multiple linear
         * regression model on the master node */
        linear_regression::training::Distributed<step2Master> masterAlgorithm;

        for (size_t i = 0; i < nBlocks; i++) {
            /* Deserialize partial results from step 1 */
            OutputDataArchive dataArch(serializedData.get() +
                                           perNodeArchLength * i,
                                       perNodeArchLength);

            linear_regression::training::PartialResultPtr
                dataForStep2FromStep1 =
                    linear_regression::training::PartialResultPtr(
                        new linear_regression::training::PartialResult());
            dataForStep2FromStep1->deserialize(dataArch);

            /* Set the local multiple linear regression model as input for the
             * master-node algorithm */
            masterAlgorithm.input.add(
                linear_regression::training::partialModels,
                dataForStep2FromStep1);
        }

        /* Merge and finalizeCompute the multiple linear regression model on the
         * master node */
        masterAlgorithm.compute();
        masterAlgorithm.finalizeCompute();

        /* Retrieve the algorithm results */
        linear_regression::training::ResultPtr trainingResult =
            masterAlgorithm.getResult();
        resultTable =
            trainingResult->get(linear_regression::training::model)->getBeta();

        printNumericTable(resultTable,
                          "Linear Regression first 20 columns of "
                          "coefficients (w0, w1..wn):",
                          1, 20);
    }
    return resultTable;
}

static NumericTablePtr ridge_regression_compute(
    int rankId, ccl::communicator &comm, const NumericTablePtr &pData,
    const NumericTablePtr &pLabel, double regParam, size_t nBlocks) {

    using daal::byte;

    NumericTablePtr ridgeParams(new HomogenNumericTable<double>(
        1, 1, NumericTable::doAllocate, regParam));

    ridge_regression::training::Distributed<step1Local> localAlgorithm;
    localAlgorithm.parameter.ridgeParameters = ridgeParams;

    /* Pass a training data set and dependent values to the algorithm */
    localAlgorithm.input.set(ridge_regression::training::data, pData);
    localAlgorithm.input.set(ridge_regression::training::dependentVariables,
                             pLabel);

    /* Train the multiple ridge regression model on local nodes */
    localAlgorithm.compute();

    /* Serialize partial results required by step 2 */
    services::SharedPtr<byte> serializedData;
    InputDataArchive dataArch;
    localAlgorithm.getPartialResult()->serialize(dataArch);
    size_t perNodeArchLength = dataArch.getSizeOfArchive();
    // std::cout << "perNodeArchLength: " << perNodeArchLength << std::endl;

    serializedData =
        services::SharedPtr<byte>(new byte[perNodeArchLength * nBlocks]);

    byte *nodeResults = new byte[perNodeArchLength];
    dataArch.copyArchiveToArray(nodeResults, perNodeArchLength);
    std::vector<size_t> aReceiveCount(comm.size(),
                                      perNodeArchLength); // 4 x "14016"

    /* Transfer partial results to step 2 on the root node */
    ccl::gather((int8_t *)nodeResults, perNodeArchLength,
                (int8_t *)(serializedData.get()), perNodeArchLength, comm)
        .wait();

    delete[] nodeResults;

    NumericTablePtr resultTable;
    if (rankId == ccl_root) {
        /* Create an algorithm object to build the final multiple ridge
         * regression model on the master node */
        ridge_regression::training::Distributed<step2Master> masterAlgorithm;

        for (size_t i = 0; i < nBlocks; i++) {
            /* Deserialize partial results from step 1 */
            OutputDataArchive dataArch(serializedData.get() +
                                           perNodeArchLength * i,
                                       perNodeArchLength);

            ridge_regression::training::PartialResultPtr dataForStep2FromStep1 =
                ridge_regression::training::PartialResultPtr(
                    new ridge_regression::training::PartialResult());
            dataForStep2FromStep1->deserialize(dataArch);

            /* Set the local multiple ridge regression model as input for the
             * master-node algorithm */
            masterAlgorithm.input.add(ridge_regression::training::partialModels,
                                      dataForStep2FromStep1);
        }

        /* Merge and finalizeCompute the multiple ridge regression model on the
         * master node */
        masterAlgorithm.compute();
        masterAlgorithm.finalizeCompute();

        /* Retrieve the algorithm results */
        ridge_regression::training::ResultPtr trainingResult =
            masterAlgorithm.getResult();
        resultTable =
            trainingResult->get(ridge_regression::training::model)->getBeta();

        printNumericTable(resultTable,
                          "Ridge Regression first 20 columns of "
                          "coefficients (w0, w1..wn):",
                          1, 20);
    }
    return resultTable;
}
#endif

#ifdef CPU_GPU_PROFILE
static jlong doLROneAPICompute(JNIEnv *env, jint rankId, jlong pData,
                               jlong pLabel, jint executorNum,
                               const ccl::string &ipPort, ComputeDevice &device,
                               jobject resultObj) {
    std::cout << "oneDAL (native): GPU compute start , rankid " << rankId
              << std::endl;
    const bool isRoot = (rankId == ccl_root);

    homogen_table xtrain = *reinterpret_cast<const homogen_table *>(pData);
    homogen_table ytrain = *reinterpret_cast<const homogen_table *>(pLabel);

    linear_regression::train_input local_input{xtrain, ytrain};
    const auto linear_regression_desc = linear_regression::descriptor<>();
    auto queue = getQueue(device);
    auto comm = preview::spmd::make_communicator<preview::spmd::backend::ccl>(
        queue, executorNum, rankId, ipPort);
    linear_regression::train_result result_train =
        preview::train(comm, linear_regression_desc, xtrain, ytrain);
    if (isRoot) {
        HomogenTablePtr result_matrix = std::make_shared<homogen_table>(
            result_train.get_model().get_betas());
        saveHomogenTablePtrToVector(result_matrix);
        return (jlong)result_matrix.get();
    } else {
        return (jlong)0;
    }
}
#endif

/*
 * Class:     com_intel_oap_mllib_regression_LinearRegressionDALImpl
 * Method:    cLinearRegressionTrainDAL
 * Signature:
 * (JJDDIIIILjava/lang/String;Lcom/intel/oap/mllib/regression/LiRResult;)J
 */
JNIEXPORT jlong JNICALL
Java_com_intel_oap_mllib_regression_LinearRegressionDALImpl_cLinearRegressionTrainDAL(
    JNIEnv *env, jobject obj, jlong data, jlong label, jdouble regParam,
    jdouble elasticNetParam, jint executorNum, jint executorCores,
    jint computeDeviceOrdinal, jint rankId, jstring ipPort, jobject resultObj) {

    std::cout << "oneDAL (native): use DPC++ kernels "
              << "; device " << ComputeDeviceString[computeDeviceOrdinal]
              << std::endl;

    const char *ipPortPtr = env->GetStringUTFChars(ipPort, 0);
    std::string ipPortStr = std::string(ipPortPtr);
    ComputeDevice device = getComputeDeviceByOrdinal(computeDeviceOrdinal);
    bool useGPU = false;
    if (device == ComputeDevice::gpu && regParam == 0) {
        useGPU = true;
    }
    NumericTablePtr resultTable;
    jlong resultptr = 0L;
    jlong ret = 0L;
    if (useGPU) {
#ifdef CPU_GPU_PROFILE
        jlong pDatagpu = (jlong)data;
        jlong pLabelgpu = (jlong)label;
        resultptr =
            doLROneAPICompute(env, rankId, pDatagpu, pLabelgpu, executorNum,
                              ipPortStr, device, resultObj);
#endif
    } else {
#ifdef CPU_ONLY_PROFILE
        ccl::communicator &comm = getComm();
        size_t rankId = comm.rank();

        NumericTablePtr pLabel = *((NumericTablePtr *)label);
        NumericTablePtr pData = *((NumericTablePtr *)data);

        // Set number of threads for oneDAL to use for each rank
        services::Environment::getInstance()->setNumberOfThreads(executorCores);

        int nThreadsNew =
            services::Environment::getInstance()->getNumberOfThreads();
        cout << "oneDAL (native): Number of CPU threads used: " << nThreadsNew
             << endl;
        if (regParam == 0) {
            resultTable = linear_regression_compute(rankId, comm, pData, pLabel,
                                                    executorNum);
        } else {
            resultTable = ridge_regression_compute(rankId, comm, pData, pLabel,
                                                   regParam, executorNum)
        }

        NumericTablePtr *coeffvectors = new NumericTablePtr(resultTable);
        resultptr = (jlong)coeffvectors;
#endif
    }

    if (rankId == ccl_root) {
        // Get the class of the result object
        jclass clazz = env->GetObjectClass(resultObj);
        // Get Field references
        jfieldID coeffNumericTableField =
            env->GetFieldID(clazz, "coeffNumericTable", "J");

        env->SetLongField(resultObj, coeffNumericTableField, resultptr);

        // intercept is already in first column of coeffvectors
        ret = resultptr;
    } else {
        ret = (jlong)0;
    }
    env->ReleaseStringUTFChars(ipPort, ipPortPtr);
    return ret;
}
