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
#include "com_sock_tcp_KernelTCPSocket.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

typedef int bool;
#define true 1
#define false 0
bool is_init = false;

void throw_new_Exception(JNIEnv *env, char *reason) {
    char *className = "java/lang/Exception";
    jclass excClass = (*env) -> FindClass(env, className);
    (*env) -> ThrowNew(env, excClass, reason);
}

JNIEXPORT jint JNICALL Java_com_sock_tcp_KernelTCPSocket_createSocketC(JNIEnv *env, jobject obj) {
    int s;
#ifdef _WIN32
    if (is_init == false){
        WSADATA wsa;
        //Initialise winsock
        printf("\nInitialising Winsock...");
        if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
        {
            printf("Failed. Error Code : %d",WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        is_init = true;
    }
#endif
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw_new_Exception(env, "cannot create socket");
    }
    return s;
}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_bindC(JNIEnv *env, jobject obj, jint sockId, jint host, jint port) {
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
#ifdef _WIN32
    sin.sin_addr.S_un.S_addr = (int)host;
#else
    sin.sin_addr.s_addr = (int)host;
#endif
    sin.sin_port = htons((int)port);

    printf("C: Try to bind() socket to port %d\n", port);
    if (bind((int)sockId, (struct sockaddr *) &sin, sizeof(sin)) < 0) 
        throw_new_Exception(env, "bind() failed, probably port is buisy");

}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_listenC(JNIEnv *env, jobject obj, jint sockId, jint backlog) {
    printf("C: try to listen()\n");
    listen(sockId, backlog);
}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_acceptC(JNIEnv *env, jobject obj, jint sockId, jobject socket) {
    struct sockaddr_in client;
    int slen = sizeof(client);
    int new_sock = accept(sockId, (struct sockaddr *) &client, &slen);
    if (new_sock < 0) {
        throw_new_Exception(env, "acception refused");
    } else {
        printf("C: new socket created with id = %d\n", new_sock);
        jclass KernelTCPSocket_class = (*env) -> GetObjectClass(env, socket);
        jfieldID fidsockId =  (*env) -> GetFieldID(env, KernelTCPSocket_class, "sockId",  "I");
        jfieldID fidaddress = (*env) -> GetFieldID(env, KernelTCPSocket_class, "address", "I");
        jfieldID fidport =    (*env) -> GetFieldID(env, KernelTCPSocket_class, "port",    "I");
        (*env) -> SetIntField(env, socket, fidsockId,  new_sock);
#ifdef _WIN32
        (*env) -> SetIntField(env, socket, fidaddress, client.sin_addr.S_un.S_addr);
#else
        (*env) -> SetIntField(env, socket, fidaddress, client.sin_addr.s_addr     );
#endif
        (*env) -> SetIntField(env, socket, fidport,    client.sin_port            );
    }

}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_connectC(JNIEnv *env, jobject obj, jint sockId, jint host, jint port) {
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
#ifdef _WIN32
    server.sin_addr.S_un.S_addr = host;
#else
    server.sin_addr.s_addr = host;
#endif

    if((connect(sockId, (struct sockaddr *) &server, sizeof(server))) != 0) {
        throw_new_Exception(env, "connection failed()");
    }
}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_sendC(JNIEnv *env, jobject obj, jint sockId, jbyteArray buf, jint bufLen, jint flag) {
    jboolean is_copy;
    char *buf_c = (*env) -> GetByteArrayElements(env, buf, &is_copy);
    int result = send(sockId, buf_c, bufLen, flag);
    (*env) -> ReleaseByteArrayElements(env, buf, buf_c, JNI_ABORT);
    if (result == -1){
        throw_new_Exception(env, "send() failed");
    }
}

JNIEXPORT jbyteArray JNICALL Java_com_sock_tcp_KernelTCPSocket_receiveC(JNIEnv *env, jobject obj, jint sockId, jint bufLen, jint flag) {
    char buf[bufLen];
    memset(buf, '\0', sizeof(buf));
    int result = recv(sockId, buf, bufLen, flag);
    if(result == -1) {
        throw_new_Exception(env, "Cannot receive failed");
    }
    printf("C: recv() message = %s\n", buf);

    // Creating java byte array and copy receiving message to them
    jbyteArray return_val = (*env) -> NewByteArray(env, bufLen);
    (*env) -> SetByteArrayRegion(env, return_val, 0, bufLen, buf);
    
    return return_val;
}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_setTimeoutReceiveC(JNIEnv *env, jobject obj, jint sockId, jint sec, jlong microsec) {
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = microsec;
    setsockopt(sockId, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(struct timeval));
}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_setTimeoutSendC(JNIEnv *env, jobject obj, jint sockId, jint sec, jlong microsec) {
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = microsec;
    setsockopt(sockId, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv, sizeof(struct timeval));
}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_setReuseAddrC(JNIEnv *env, jobject obj, jint sockId, jint flag) {
    int reuse = flag;
    if (setsockopt(sockId, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse)) < 0){
        throw_new_Exception(env, "cannot set SO_REUSEADDR option");
    }
}

JNIEXPORT void JNICALL Java_com_sock_tcp_KernelTCPSocket_closeC(JNIEnv *env, jobject obj, jint sockId) {
    close(sockId);
}
