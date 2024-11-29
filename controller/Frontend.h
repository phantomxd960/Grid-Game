/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class Frontend */

#ifndef _Included_Frontend
#define _Included_Frontend
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Frontend
 * Method:    startLevel1
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Frontend_startLevel1
  (JNIEnv *, jobject);

/*
 * Class:     Frontend
 * Method:    startLevel2
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Frontend_startLevel2
  (JNIEnv *, jobject);

/*
 * Class:     Frontend
 * Method:    startLevel3
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Frontend_startLevel3
  (JNIEnv *, jobject);

/*
 * Class:     Frontend
 * Method:    movePlayer
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Frontend_movePlayer
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     Frontend
 * Method:    performAction
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Frontend_performAction
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Frontend
 * Method:    performDragonAction
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Frontend_performDragonAction
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     Frontend
 * Method:    performStoreAction
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Frontend_performStoreAction
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Frontend
 * Method:    performCaveAction
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_Frontend_performCaveAction
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Frontend
 * Method:    exitGame
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Frontend_exitGame
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
