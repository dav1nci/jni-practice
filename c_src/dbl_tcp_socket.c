#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include <dbl.h>
#include "com_sock_tcp_DBLTCPSocket.h"


JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpSendC(JNIEnv *env, jobject obj, jint channId, jbyteArray buf, jint bufLen, jint flag) {
    
}

JNIEXPORT void JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpAcceptC(JNIEnv *env, jobject obj, jint channId, jobject newSocket) {

}

JNIEXPORT void JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpListenC(JNIEnv *env, jobject obj, jint channId) {

}

JNIEXPORT jbyteArray JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpReceiveC(JNIEnv *env, jobject obj, jint channId, jint rcvMode, jint bufLen, jobject rcvInfo) {

}

JNIEXPORT jobjectArray JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpReceiveMsgC(JNIEnv *env, jobject obj, jint devId, jint rcvMode, jint rcvMax) {

}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpPollC(JNIEnv *env, jobject obj, jintArray channels, jint arrLen, jint timeout) {

}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_getChannelOptionsC(JNIEnv *env, jobject obj, jint channId, jint level, jint optName) {

}

JNIEXPORT void JNICALL Java_com_sock_tcp_DBLTCPSocket_setChannelOptionsC(JNIEnv *env, jobject obj, jint channId, jint level, jint optName, jint optVal) {

}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_getChannelTypeC(JNIEnv *env, jobject obj, jint channId) {

}
