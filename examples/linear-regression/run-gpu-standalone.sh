#!/usr/bin/env bash

source ../../conf/env.sh

# Data file is from Spark Examples (data/mllib/sample_linear_regression_data.txt) and put in examples/data
# The data file should be copied to $HDFS_ROOT before running examples
DATA_FILE=/home/kunpengj/data/LinearRegression/yearPredictionMSD/YearPredictionMSDraw

APP_JAR=target/oap-mllib-examples-$OAP_MLLIB_VERSION.jar
APP_CLASS=org.apache.spark.examples.ml.LinearRegressionExample

DEVICE=CPU
RESOURCE_FILE=$PWD/IntelGpuResourceFile.json
WORKER_GPU_AMOUNT=1
EXECUTOR_GPU_AMOUNT=1
TASK_GPU_AMOUNT=1

# Should run in standalone mode
time $SPARK_HOME/bin/spark-submit --master $SPARK_MASTER \
    --num-executors 1 \
    --executor-cores $SPARK_EXECUTOR_CORES \
    --total-executor-cores $SPARK_TOTAL_CORES \
    --driver-memory $SPARK_DRIVER_MEMORY \
    --executor-memory $SPARK_EXECUTOR_MEMORY \
    --conf "spark.serializer=org.apache.spark.serializer.KryoSerializer" \
    --conf "spark.default.parallelism=$SPARK_DEFAULT_PARALLELISM" \
    --conf "spark.sql.shuffle.partitions=$SPARK_DEFAULT_PARALLELISM" \
    --conf "spark.driver.extraClassPath=$SPARK_DRIVER_CLASSPATH" \
    --conf "spark.executor.extraClassPath=$SPARK_EXECUTOR_CLASSPATH" \
    --conf "spark.oap.mllib.enabled=true" \
    --conf "spark.oap.mllib.device=GPU" \
    --conf "spark.oap.mllib.isTest=false" \
    --conf "spark.worker.resourcesFile=$RESOURCE_FILE" \
    --conf "spark.worker.resource.gpu.amount=$WORKER_GPU_AMOUNT" \
    --conf "spark.executor.resource.gpu.amount=$EXECUTOR_GPU_AMOUNT" \
    --conf "spark.task.resource.gpu.amount=$TASK_GPU_AMOUNT" \
    --conf "spark.shuffle.reduceLocality.enabled=false" \
    --conf "spark.network.timeout=1200s" \
    --conf "spark.task.maxFailures=1" \
    --jars $OAP_MLLIB_JAR \
    --class $APP_CLASS \
    $APP_JAR $DATA_FILE \
    2>&1 | tee LinearRegression-$(date +%m%d_%H_%M_%S).log

