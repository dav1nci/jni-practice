#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif
#include <dbl.h>
#include "com_sock_udp_DBLUDPSocket.h"

#define DBL_Safe(x) do {                              \
        int ret = (x);                                    \
        if (ret != 0) {                                   \
            fprintf(stderr, "DBL Failure at line %d: %s (errno=%d)\n", \
                __LINE__, strerror(ret), ret);                   \
            exit(-1);                                       \
        }                                                 \
} while (0)

//#ifdef _WIN32
//#pragma comment(lib, "ws2_32.lib")
//#endif

dbl_device_t **devices;
dbl_channel_t **channels;
dbl_send_t **send_handles;
dbl_send_t sendh;

int ch_id = 0;

enum dbl_recvmode recv_mode;

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_init(JNIEnv *env, jclass class){
    DBL_Safe(dbl_init(DBL_VERSION_API));
    devices = (dbl_device_t **) realloc(devices, sizeof(dbl_device_t *));
    devices[0] = (dbl_device_t *) malloc(sizeof(dbl_device_t));
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_createSocketC(JNIEnv *env, jobject obj, jint host){
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
#ifdef _WIN32
    sin.sin_addr.S_un.S_addr = (int)host;
#else
    sin.sin_addr.s_addr = (int)host;
#endif
    DBL_Safe(dbl_open(&sin.sin_addr, 0, devices[0]));
    channels = (dbl_channel_t **) realloc(channels, sizeof(dbl_channel_t *));
    channels[ch_id] = (dbl_channel_t *) malloc(sizeof(dbl_channel_t));
    ch_id++;
    return ch_id - 1;
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_sendC(JNIEnv *env, jobject obj, jint sockId, jbyteArray buf, jint bufLen, jint host, jint port){
    struct sockaddr_in remote;
    memset((char *) &remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons((int)port);
#ifdef _WIN32
    remote.sin_addr.S_un.S_addr = (int)host;//inet_addr(c_host);
#else
    remote.sin_addr.s_addr = (int)host;//inet_addr(c_host);
#endif

    jboolean is_copy;
    char *buf_c = (*env) -> GetByteArrayElements(env, buf, &is_copy);
    printf("Trying to send ");

    DBL_Safe(dbl_send_connect((*channels[0]), &remote, 0, 0, &sendh));

    DBL_Safe(dbl_send(sendh, buf_c, (int)bufLen, 0));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_bindC(JNIEnv *env, jobject obj, jint sockId, jint port){
    DBL_Safe(dbl_bind((*devices[0]), 0, (int)port, NULL, channels[(int)sockId]));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_closeC(JNIEnv *env, jobject obj, jint sockId){
    DBL_Safe(dbl_unbind((*channels[sockId])));
    //dbl_close(devices[0]);
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_receiveC(JNIEnv *env, jobject obj, jint sockId, jobject packet, jint bufLen){
    char buf[bufLen];
    struct dbl_recv_info rxinfo;
    DBL_Safe(dbl_recvfrom((*devices[0]), DBL_RECV_DEFAULT, buf, bufLen, &rxinfo));
    jclass udp_packet_class = (*env) -> GetObjectClass(env, packet);

    jfieldID fidAddress = (*env) -> GetFieldID(env, udp_packet_class, "address", "Ljava/net/InetAddress;");
    jfieldID fidBuf =     (*env) -> GetFieldID(env, udp_packet_class, "buf", "[B");
    jfieldID fidPort =    (*env) -> GetFieldID(env, udp_packet_class, "port", "I");


    jclass inet_address_class = (*env) -> FindClass(env, "java/net/InetAddress");

    jmethodID inet_addr_getByName = (*env) -> GetStaticMethodID(env, inet_address_class, "getByName", "(Ljava/lang/String;)Ljava/net/InetAddress;");
    jstring host = (*env) -> NewStringUTF(env, inet_ntoa(rxinfo.sin_from.sin_addr));
    jobject inet_address = (*env) -> CallStaticObjectMethod(env, inet_address_class, inet_addr_getByName, host);

    jbyteArray message = (*env) -> NewByteArray(env, (int)bufLen);
    (*env) -> SetByteArrayRegion(env, message, 0, (int)bufLen, buf);
    (*env) -> SetObjectField(    env, packet,  fidAddress,     inet_address);
    (*env) -> SetObjectField(    env, packet,  fidBuf,         message);
    (*env) -> SetIntField(       env, packet,  fidPort,        ntohs(rxinfo.sin_from.sin_port));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_connectC(JNIEnv *env, jobject obj, jint sockId, jint host, jint port){
    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons((int)port);
    remote.sin_addr.s_addr = (int)host;
    dbl_send_t sh;
    //dbl_send_connect(*channels[(int)sockId], &remote, 0, 0, &sh);
}

