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

JNIEXPORT jint JNICALL Java_com_sock_udp_KernelUDPSocket_createSocketC(JNIEnv *env, jobject obj){
    int s;
#ifdef _WIN32
    WSADATA wsa;
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");
#endif
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Cannot create socket\n");
        (*env) -> ThrowNew(env, excClass, "Creation socket failed");
    }
    return s;
}

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_sendC(
        JNIEnv *env, 
        jobject obj, 
        jint sock_id, 
        jbyteArray message, 
        jint mess_len,
        jint host, 
        jint port){
    struct sockaddr_in receiver;
    memset((char *) &receiver, 0, sizeof(receiver));

    //printf("C: try to send on host %d\n", (int)host);
#ifdef _WIN32
    receiver.sin_addr.S_un.S_addr = (int)host;//inet_addr(c_host);
#else
    receiver.sin_addr.s_addr = (int)host;//inet_addr(c_host);
#endif
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons((int)port);

    //printf("C: Sending a message...\n");
    jboolean is_copy;
    jint length = (*env) -> GetArrayLength(env, message);
    char *message_c = (*env) -> GetByteArrayElements(env, message, &is_copy);
    if (sendto(sock_id, message_c, (int)length, 0, (struct sockaddr *) &receiver, sizeof(receiver)) == -1){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Sending failed!\n");
        (*env) -> ThrowNew(env, excClass, "Sending Faied");
    }
    //printf("C: Message sended\n");
}


JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_bindC(JNIEnv *env, jobject obj, jint sockId, jint port){
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons((int)port);

    if (bind((int)sockId, (struct sockaddr *) &server, sizeof(server)) < 0){
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

JNIEXPORT void JNICALL Java_com_sock_udp_KernelUDPSocket_receiveC(JNIEnv *env, jobject obj, jint sockId, jobject packet, jint buflen){

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
    conn_sock.sin_addr.s_addr = (int)host;
    conn_sock.sin_port = htons((int)port);

    if (connect((int)sockId, (struct sockaddr *) &conn_sock, sizeof(conn_sock)) < 0){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Connection failed\n");
        (*env) -> ThrowNew(env, excClass, "Connection failed");
    }
}
