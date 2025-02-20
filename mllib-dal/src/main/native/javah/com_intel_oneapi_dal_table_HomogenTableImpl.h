/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_intel_oneapi_dal_table_HomogenTableImpl */

#ifndef _Included_com_intel_oneapi_dal_table_HomogenTableImpl
#define _Included_com_intel_oneapi_dal_table_HomogenTableImpl
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    iInit
 * Signature: (JJ[III)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_iInit
  (JNIEnv *, jobject, jlong, jlong, jintArray, jint, jint);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    fInit
 * Signature: (JJ[FII)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_fInit
  (JNIEnv *, jobject, jlong, jlong, jfloatArray, jint, jint);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    dInit
 * Signature: (JJ[DII)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_dInit
  (JNIEnv *, jobject, jlong, jlong, jdoubleArray, jint, jint);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    lInit
 * Signature: (JJ[JII)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_lInit
  (JNIEnv *, jobject, jlong, jlong, jlongArray, jint, jint);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cEmptyTableInit
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cEmptyTableInit
  (JNIEnv *, jobject);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetColumnCount
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetColumnCount
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetRowCount
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetRowCount
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetKind
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetKind
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetDataLayout
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetDataLayout
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetMetaData
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetMetaData
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetPullRowsIface
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetPullRowsIface
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetPullColumnIface
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetPullColumnIface
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetPullCSRBlockIface
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetPullCSRBlockIface
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetIntData
 * Signature: (J)[I
 */
JNIEXPORT jintArray JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetIntData
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetLongData
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetLongData
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetFloatData
 * Signature: (J)[F
 */
JNIEXPORT jfloatArray JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetFloatData
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cGetDoubleData
 * Signature: (J)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cGetDoubleData
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_intel_oneapi_dal_table_HomogenTableImpl
 * Method:    cAddHomogenTable
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_oneapi_dal_table_HomogenTableImpl_cAddHomogenTable
  (JNIEnv *, jobject, jlong, jlong, jint);

#ifdef __cplusplus
}
#endif
#endif
