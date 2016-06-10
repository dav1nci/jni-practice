#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "com_udp_UDPDemo.h"

#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT 8888

JNIEXPORT jbyteArray JNICALL Java_com_udp_UDPDemo_sendMessage(JNIEnv *env, jclass cls, jbyteArray arr){
    char buf[BUFLEN];
    char message[BUFLEN];

    struct sockaddr_in server_addr;
    int s, slen = sizeof(server_addr);

    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_aton(SERVER, &server_addr.sin_addr) == 0){
        fprintf(stderr, "inet_aton() failed \n");
        exit(1);
    }

    jboolean is_copy;
    jint length = (*env) -> GetArrayLength(env, arr);
    char *mes = (*env) -> GetByteArrayElements(env, arr, &is_copy);

    //send message
    sendto(s, mes, (int)length, 0, (struct sockaddr *) &server_addr, sizeof(server_addr));

    //receive a reply and print it
    memset(buf, '\0', BUFLEN);
    //receive some data
    recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &server_addr, &slen);
    close(s);
    jbyteArray result = (*env) -> NewByteArray(env, BUFLEN);

    (*env) -> SetByteArrayRegion(env, result, 0, (int)length, buf);
    return result;
}
