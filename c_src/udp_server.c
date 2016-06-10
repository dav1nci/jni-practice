#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define BUFLEN 512  
#define PORT 8888

int main(void){
    struct sockaddr_in server, client;
    int s, slen = sizeof(client);
    char buf[BUFLEN];
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset((char *) &server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s, (struct sockaddr*)&server, sizeof(server));

    printf("Waiting for data .....");
    fflush(stdout);
    int recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &client, &slen);
    printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    printf("Data: %s\n", buf);
    sendto(s, buf, recv_len, 0, (struct sockaddr*) &client, slen);
    close(s);
    return 0;
}
