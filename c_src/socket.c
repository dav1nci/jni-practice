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
        printf("Cannot create socket\n");
        return -1;
    }
    return s;
}

JNIEXPORT void JNICALL Java_com_sock_udp_UDPSocket_sendC(
        JNIEnv *env, 
        jobject obj, 
        jint sock_id, 
        jbyteArray message, 
        jint mess_len,
        jstring host, 
        jint port){
    struct sockaddr_in receiver;
    // get host from Java String
    const char *c_host = (*env)->GetStringUTFChars(env, host, NULL);

    receiver.sin_addr.s_addr = inet_addr(c_host);
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons((int)port);

    printf("C: Sending a message...\n");
    jboolean is_copy;
    jint length = (*env) -> GetArrayLength(env, message);
    char *message_c = (*env) -> GetByteArrayElements(env, message, &is_copy);
    if (sendto(sock_id, message_c, (int)length, 0, (struct sockaddr *) &receiver, sizeof(receiver)) == -1)
        printf("Sending failed!\n");
}


JNIEXPORT void JNICALL Java_com_sock_udp_UDPSocket_bindC(JNIEnv *env, jobject obj, jint sockId, jint port){
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons((int)port);

    if (bind((int)sockId, (struct sockaddr *) &server, sizeof(server)) < 0)
        printf("C: bind failed\n");
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
        printf("Cant receive data from socket!\n");
        return 0;
    }
    jbyteArray result = (*env) -> NewByteArray(env, (int)buflen);
    (*env) -> SetByteArrayRegion(env, result, 0, (int)buflen, buf);
    return result;
}

