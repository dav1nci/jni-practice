/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_sock_udp_KernelUDPSocket */

#ifndef _Included_com_sock_udp_KernelUDPSocket
#define _Included_com_sock_udp_KernelUDPSocket
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_sock_udp_KernelUDPSocket
 * Method:    createSocketC
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_sock_udp_KernelUDPSocket_createSocketC
  (JNIEnv *, jobject);

/*
 * Class:     com_sock_udp_KernelUDPSocket
 * Method:    sendC
 * Signature: (I[BIII)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_sendC
  (JNIEnv *, jobject, jint, jbyteArray, jint, jint, jint);

/*
 * Class:     com_sock_udp_KernelUDPSocket
 * Method:    bindC
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_bindC
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     com_sock_udp_KernelUDPSocket
 * Method:    closeC
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_closeC
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_sock_udp_KernelUDPSocket
 * Method:    receiveC
 * Signature: (ILcom/sock/udp/UDPPacket;I)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_receiveC
  (JNIEnv *, jobject, jint, jobject, jint);

/*
 * Class:     com_sock_udp_KernelUDPSocket
 * Method:    connectC
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_connectC
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     com_sock_udp_KernelUDPSocket
 * Method:    disconnectC
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_disconnectC
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
