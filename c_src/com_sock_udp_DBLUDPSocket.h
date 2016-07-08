/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_sock_udp_DBLUDPSocket */

#ifndef _Included_com_sock_udp_DBLUDPSocket
#define _Included_com_sock_udp_DBLUDPSocket
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_init
  (JNIEnv *, jclass);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    createDeviceC
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_createDeviceC
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    sendC
 * Signature: (I[BII)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_sendC
  (JNIEnv *, jobject, jint, jbyteArray, jint, jint);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    sendToC
 * Signature: (III[BII)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_sendToC
  (JNIEnv *, jobject, jint, jint, jint, jbyteArray, jint, jint);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    sendConnectC
 * Signature: (IIIII)I
 */
JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_sendConnectC
  (JNIEnv *, jobject, jint, jint, jint, jint, jint);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    bindC
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_bindC
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    receiveFromC
 * Signature: (IILcom/sock/udp/UDPPacket;I)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_receiveFromC
  (JNIEnv *, jobject, jint, jint, jobject, jint);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    shutdownC
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_shutdownC
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    unbindC
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_unbindC
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_sock_udp_DBLUDPSocket
 * Method:    closeC
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_closeC
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
