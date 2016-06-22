#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include "com_sock_udp_UDPSocket.h"
#include <unistd.h> // for close()

JNIEXPORT jint JNICALL Java_com_sock_udp_UDPSocket_createSocketC(JNIEnv *env, jobject obj){
    int s;
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Cannot create socket\n");
        (*env) -> ThrowNew(env, excClass, "Creation socket failed");
    }
    return s;
}

JNIEXPORT void JNICALL Java_com_sock_udp_UDPSocket_sendC(
        JNIEnv *env, 
        jobject obj, 
        jint sock_id, 
        jbyteArray message, 
        jint mess_len,
        jint host, 
        jint port){
    struct sockaddr_in receiver;
    
    printf("C: try to send on host %d\n", (int)host);
    receiver.sin_addr.s_addr = (int)host;//inet_addr(c_host);
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons((int)port);

    printf("C: Sending a message...\n");
    jboolean is_copy;
    jint length = (*env) -> GetArrayLength(env, message);
    char *message_c = (*env) -> GetByteArrayElements(env, message, &is_copy);
    if (sendto(sock_id, message_c, (int)length, 0, (struct sockaddr *) &receiver, sizeof(receiver)) == -1){
        char *className = "java/lang/Exception";
        jclass excClass = (*env) -> FindClass(env, className);
        printf("C: Sending failed!\n");
        (*env) -> ThrowNew(env, excClass, "Sending Faied");
    }
    printf("C: Message sended\n");
}


JNIEXPORT void JNICALL Java_com_sock_udp_UDPSocket_bindC(JNIEnv *env, jobject obj, jint sockId, jint port){
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

JNIEXPORT void JNICALL Java_com_sock_udp_UDPSocket_closeC(JNIEnv *env, jobject obj, jint sockId){
    close((int)sockId);
}

JNIEXPORT jbyteArray JNICALL Java_com_sock_udp_UDPSocket_receiveC(JNIEnv *env, jobject obj, jint sockId, jint buflen){
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
    jbyteArray result = (*env) -> NewByteArray(env, (int)buflen);
    (*env) -> SetByteArrayRegion(env, result, 0, (int)buflen, buf);
    return result;
}

