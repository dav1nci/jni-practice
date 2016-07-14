#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "com_sock_udp_KernelUDPSocket.h"
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h> // for close()
#endif

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

typedef int bool;
#define true 1
#define false 0
bool is_init = false;

JNIEXPORT jint JNICALL Java_com_sock_udp_KernelUDPSocket_createSocketC(JNIEnv *env, jobject obj){
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
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
    {
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Cannot create socket\n");
        (*env) -> ThrowNew(env, excClass, "Creation socket failed");
    }
    return s;
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_sendToC(
        JNIEnv *env, 
        jobject obj, 
        jint sock_id, 
        jbyteArray message, 
        jint mess_len,
        jint host, 
        jint port){
    struct sockaddr_in receiver;
    memset((char *) &receiver, 0, sizeof(receiver));
#ifdef _WIN32
    receiver.sin_addr.S_un.S_addr = (int)host;//inet_addr(c_host);
#else
    receiver.sin_addr.s_addr = (int)host;//inet_addr(c_host);
#endif
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons((int)port);

    //printf("C: Sending a message...\n");
    jboolean is_copy;
    char *message_c = (*env) -> GetByteArrayElements(env, message, &is_copy);
    int result = sendto(sock_id, message_c, (int)mess_len, 0, (struct sockaddr *) &receiver, sizeof(receiver));
    (*env) -> ReleaseByteArrayElements(env, message, message_c, JNI_ABORT);
    if (result == -1) {
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: sendto() failed!\n");
        (*env) -> ThrowNew(env, excClass, "Sending Faied");
    }
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_sendC(JNIEnv *env, jobject obj, jint sockId, jbyteArray buf, jint bufLen){
    jboolean is_copy;
    char *buf_c = (*env) -> GetByteArrayElements(env, buf, &is_copy);
    int result = send(sockId, buf_c, bufLen, 0);
    (*env) -> ReleaseByteArrayElements(env, buf, buf_c, JNI_ABORT);
    if (result == -1){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: send() failed!\n");
        (*env) -> ThrowNew(env, excClass, "Sending Faied");
    }
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_bindC(JNIEnv *env, jobject obj, jint sockId, jint host, jint port){
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
#ifdef _WIN32
    sin.sin_addr.S_un.S_addr = (int)host;
#else
    sin.sin_addr.s_addr = (int)host;
#endif
    sin.sin_port = htons((int)port);

    if (bind((int)sockId, (struct sockaddr *) &sin, sizeof(sin)) < 0){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Bind failed\n");
        (*env) -> ThrowNew(env, excClass, "Bind failed");
    }
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_closeC(JNIEnv *env, jobject obj, jint sockId){
#ifdef _WIN32
    closesocket((int)sockId);
    WSACleanup();
#else
    close((int)sockId);
#endif
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_receiveC(JNIEnv *env, jobject obj, jint sockId, jobject packet, jint bufLen){
   char buf[bufLen];
   memset(buf, '\0', sizeof(buf));
   int result = recv(sockId, buf, bufLen, 0);
   if (result == -1){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: recv() failed\n");
        (*env) -> ThrowNew(env, excClass, "recv() failed");
    }
    
    jclass udp_packet_class = (*env) -> GetObjectClass(env, packet);
    jfieldID fidBuf =     (*env) -> GetFieldID(env, udp_packet_class, "buf", "[B");

    // Creating java byte array and copy receiving message to them
    jbyteArray message = (*env) -> NewByteArray(env, bufLen);
    (*env) -> SetByteArrayRegion(env, message, 0, bufLen, buf);

    // Setting packet buf field
    (*env) -> SetObjectField(env, packet, fidBuf, message);
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_receiveFromC(JNIEnv *env, jobject obj, jint sockId, jobject packet, jint buflen){
    int recv_len;
    char buf[(int)buflen];
    memset(buf, '\0', (int)buflen);
    struct sockaddr_in other;
    int sock_len = sizeof(other);
    if ((recv_len = recvfrom((int)sockId, buf, (int)buflen, 0, (struct sockaddr *) &other, &sock_len)) == -1){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Cannot receive message\n");
        (*env) -> ThrowNew(env, excClass, "Cannot receive message");
    }
    jclass udp_packet_class = (*env) -> GetObjectClass(env, packet);

    jfieldID fidAddress = (*env) -> GetFieldID(env, udp_packet_class, "address", "Ljava/net/InetAddress;");
    jfieldID fidBuf =     (*env) -> GetFieldID(env, udp_packet_class, "buf", "[B");
    jfieldID fidPort =    (*env) -> GetFieldID(env, udp_packet_class, "port", "I");

    ////Getting InetAddress object

    jclass inet_address_class = (*env) -> FindClass(env, "java/net/InetAddress");

    jmethodID inet_addr_getByName = (*env) -> GetStaticMethodID(env, inet_address_class, "getByName", "(Ljava/lang/String;)Ljava/net/InetAddress;");
    jstring host = (*env) -> NewStringUTF(env, inet_ntoa(other.sin_addr));
    jobject inet_address = (*env) -> CallStaticObjectMethod(env, inet_address_class, inet_addr_getByName, host);

    (*env) -> ReleaseStringUTFChars(env, host, NULL);

    jbyteArray message = (*env) -> NewByteArray(env, (int)buflen);
    (*env) -> SetByteArrayRegion(env, message, 0, (int)buflen, buf);
    (*env) -> SetObjectField(    env, packet,  fidAddress,     inet_address);
    (*env) -> SetObjectField(    env, packet,  fidBuf,         message);
    (*env) -> SetIntField(       env, packet,  fidPort,        ntohs(other.sin_port));

    //printf("Received packet from %s:%d\n", inet_ntoa(other.sin_addr), ntohs(other.sin_port));
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_connectC(JNIEnv *env, jobject obj, jint sockId, jint host, jint port){
    struct sockaddr_in conn_sock;
    conn_sock.sin_family = AF_INET;
#ifdef _WIN32
    conn_sock.sin_addr.S_un.S_addr = (int)host;
#else
    conn_sock.sin_addr.s_addr = (int)host;
#endif
    conn_sock.sin_port = htons((int)port);

    if (connect((int)sockId, (struct sockaddr *) &conn_sock, sizeof(conn_sock)) < 0){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Connection failed\n");
        (*env) -> ThrowNew(env, excClass, "Connection failed");
    }
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_disconnectC(JNIEnv *env, jobject obj, jint sockId){
#ifdef _WIN32
    shutdown(sockId, SD_SEND);
#else
    shutdown(sockId, SHUT_RDWR);
#endif
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_joinMcastGroupC(JNIEnv *env, jobject obj, jint sockId, jint mcastIp, jint interfaceIp){
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = mcastIp;
    group.imr_interface.s_addr = interfaceIp;
    if (setsockopt(sockId, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &group, sizeof(group)) < 0){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: adding socket to mcast group faied\n");
        (*env) -> ThrowNew(env, excClass, "Adding socket to mcast group faied");
    }

}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_leaveMcastGroup(JNIEnv *env, jobject obj, jint sockId, jint mcastIp, jint interfaceIp){
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = mcastIp;
    group.imr_interface.s_addr = interfaceIp;
    if (setsockopt(sockId, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *) &group, sizeof(group)) < 0){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: removing socket from mcast group faied\n");
        (*env) -> ThrowNew(env, excClass, "Removing socket from mcast group faied");
    }
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_setReuseAddrC(JNIEnv *env, jobject obj, jint sockId, jint flag){
    int reuse = flag;
    if (setsockopt(sockId, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse)) < 0){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: SET SO_REUSEADDR failed\n");
        (*env) -> ThrowNew(env, excClass, "Set SO_REUSEADDR failed");
    }
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_setTimeout(JNIEnv *env, jobject obj, jint sockId, jint n, jint flag){
    struct timeval tv;
    if (flag == 0){
        tv.tv_sec = n;
        tv.tv_usec = 0;
    } else {
        tv.tv_sec = 0;
        tv.tv_usec = n;
    }
    if (setsockopt(sockId, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: setting timeout failed\n");
        (*env) -> ThrowNew(env, excClass, "Setting timeout failed");
    }
}
