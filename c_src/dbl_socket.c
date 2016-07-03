#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include <dbl.h>
#include "com_sock_udp_DBLUDPSocket.h"

//#ifdef _WIN32
//#pragma comment(lib, "ws2_32.lib")
//#endif

#define DBL_Safe(x) do {                              \
    int ret = (x);                                    \
    if (ret != 0) {                                   \
        fprintf(stderr, "DBL Failure at line %d: %s (errno=%d)\n", \
                __LINE__, strerror(ret), ret);                   \
        exit(-1);                                       \
    }                                                 \
} while (0)

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_init(JNIEnv *env, jclass class){
    printf("C: try to init dbl\n");
    printf("DBL_VERSION_API is %d\n", DBL_VERSION_API);
    printf("C: dbl_init returned %d\n", dbl_init(DBL_VERSION_API));
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_createSocketC(JNIEnv *env, jobject obj){
    DBL_Safe(dbl_init(DBL_VERSION_API));
    typeof(dbl_device_t);
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_sendC(JNIEnv *env, jobject obj, jint sockId, jbyteArray buf, jint bufLen, jint host, jint port){

}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_bindC(JNIEnv *env, jobject obj, jint sockId, jint port){

}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_closeC(JNIEnv *env, jobject obj, jint sockId){

}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_receiveC(JNIEnv *env, jobject obj, jint sockId, jobject packet, jint bufLen){

}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_connectC(JNIEnv *env, jobject obj, jint sockId, jint host, jint port){

}

