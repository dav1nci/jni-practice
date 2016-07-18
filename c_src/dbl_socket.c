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

typedef int bool;
#define true 1
#define false 0

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

dbl_device_t **devices;
dbl_channel_t **channels;
dbl_send_t **send_handles;
dbl_device_t dev;

int channels_num = 0;
int devices_num = 0;
int send_handles_num = 0;

enum dbl_recvmode rmode = DBL_RECV_DEFAULT;

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_init(JNIEnv *env, jclass class){
    //char *desc = "Description:\n\tdbl_pingpong\n\tMeasures UDP pingpong latency using the DBL API.\n\tWhen the client and server machines are connected back-to-back (switchless),\n\tthe expected half-round trip latency is 3 to 4 microseconds.\n\nExample:\n\t[server]\n\t\t./dbl_pingpong -s -l 192.168.1.1 -p 3333 -i 10000\n\n\t[client]\n\t\t./dbl_pingpong -h 192.168.1.1 -l 192.168.1.2 -p 3333 -i 10000\n";
    //printf("Description: \n%s", desc);
    printf("Try to dbl_init()\n");
    DBL_Safe(dbl_init(DBL_VERSION_API));
    return 0;
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_createDeviceC(JNIEnv *env, jobject obj, jint host, jint flag){
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
#ifdef _WIN32
    sin.sin_addr.S_un.S_addr = (int)host;
#else
    sin.sin_addr.s_addr = (int)host;
#endif
    printf("C: Create device with host %d\n", host);


    // Get memory for new device
    devices = (dbl_device_t **) realloc(devices, (devices_num + 1) * sizeof(dbl_device_t *));
    devices[devices_num] = (dbl_device_t *) malloc(sizeof(dbl_device_t));


    // Create a new dvice
    printf("C: Try dbl_open() dev num is : %d\n", devices_num);
    DBL_Safe(dbl_open(&sin.sin_addr, 0, devices[devices_num]));
    devices_num++;
    return devices_num - 1;
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_sendC(JNIEnv *env, jobject obj, jint handleId, jbyteArray buf, jint bufLen, jint flag){
    jboolean is_copy;
    char *buf_c = (*env) -> GetByteArrayElements(env, buf, &is_copy);
    printf("C: try to dbl_send() message: %s with send_handles_id = %d\n", buf_c, handleId);
    DBL_Safe(dbl_send((*send_handles[handleId]), buf_c, (int)bufLen, flag));
    (*env) -> ReleaseByteArrayElements(env, buf, buf_c, JNI_ABORT);
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_sendToC(JNIEnv *env, jobject obj, jint channId, jint host, jint port, jbyteArray buf, jint bufLen, jint flag){
    assert((int) bufLen > 1);
    struct sockaddr_in remote;
    memset(&remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;
    remote.sin_port = htons((int)port);
#ifdef _WIN32
    remote.sin_addr.S_un.S_addr = (int)host;
#else
    remote.sin_addr.s_addr = (int)host;
#endif

    jboolean is_copy;
    char *buf_c = (*env) -> GetByteArrayElements(env, buf, &is_copy);
    printf("C: try to dbl_send_to() message: %s with channId = %d host = %d, port = %d flag = %d\n", buf_c, channId, host, port, 0);
    DBL_Safe(dbl_sendto((*channels[channId]), &remote, buf_c, (int)bufLen, 0));
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_sendConnectC(JNIEnv *env, jobject obj, jint channId, jint host, jint port, jint flag, jint ttl){
    assert((int)channId >= 0);
    assert((int)send_handles_num >= 0);
    // Get memory for new connect_handler
    send_handles = (dbl_send_t **) realloc(send_handles, (send_handles_num + 1) * sizeof(dbl_send_t *));
    assert((int)send_handles_num >= 0);
    send_handles[send_handles_num] = (dbl_send_t *) malloc(sizeof(dbl_send_t));
    send_handles_num++;

    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons((int)port);
#ifdef _WIN32
    remote.sin_addr.S_un.S_addr = (int)host;
#else
    remote.sin_addr.s_addr = (int)host;
#endif
    //dbl_channel_t ch;
    //dbl_send_t sh;
    //printf("C: try to dbl_dind() test\n");
    //DBL_Safe(dbl_bind((*devices[0]), 0, (int)port + 1, NULL, &ch));
    printf("C: Try to dbl_send_connect() on channelId = %d, send_handles = %d\n", (int)channId, (send_handles_num - 1));
    DBL_Safe(dbl_send_connect((*channels[(int)channId]), &remote, 0, 0, send_handles[send_handles_num - 1]));
    //DBL_Safe(dbl_send_connect(ch, &remote, 0, 0, &sh));
    return send_handles_num - 1;
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_bindC(JNIEnv *env, jobject obj, jint devId, jint flag, jint port){
    assert((int)devId >= 0);
    assert(channels_num >= 0);
    channels = (dbl_channel_t **) realloc(channels, (channels_num + 1) * sizeof(dbl_channel_t *));
    channels[channels_num] = (dbl_channel_t *) malloc(sizeof(dbl_channel_t));
    channels_num++;
    printf("C: Try to dbl_bind() on port %d with device %d\n", port, (int)devId);
    DBL_Safe(dbl_bind((*devices[(int)devId]), 0, (int)port, NULL, channels[channels_num - 1]));
    //dbl_channel_t ch;
    //DBL_Safe(dbl_bind(dev, 0, (int)port, NULL, &ch));
    return channels_num - 1;
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_bindAddrC(JNIEnv *env, jobject obj, jint devId, jint interfaceIp, jint flag, jint port){
    assert((int)devId >= 0);
    assert(channels_num >= 0);
    struct in_addr interface_ip;
    memset(&interface_ip, 0, sizeof(interface_ip));
#ifdef _WIN32
    interface_ip.S_un.S_addr = (int)interfaceIp;
#else
    interface_ip.s_addr = (int)interfaceIp;
#endif
    channels = (dbl_channel_t **) realloc(channels, (channels_num + 1) * sizeof(dbl_channel_t *));
    channels[channels_num] = (dbl_channel_t *) malloc(sizeof(dbl_channel_t));
    channels_num++;
    printf("C: Try to dbl_bind_addr() on port %d with device %d on interface %d\n", port, (int)devId, interfaceIp);
    DBL_Safe(dbl_bind_addr((*devices[(int)devId]), &interface_ip, flag, port, NULL, channels[channels_num - 1]));
    return channels_num - 1;
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_receiveFromC(JNIEnv *env, jobject obj, jint devId, jint recvMode, jobject packet, jint bufLen){
    assert((int)devId >= 0);
    char buf[(int)bufLen];
    struct dbl_recv_info rxinfo;
    enum dbl_recvmode rmode = recvMode;
    printf("C: Try to dbl_recvfrom() from dev = %d\n", (int)devId);
    DBL_Safe(dbl_recvfrom((*devices[(int)devId]), rmode, buf, bufLen, &rxinfo));
    printf("C: After dbl_recvfrom() from dev = %d\n", (int)devId);
    jclass udp_packet_class = (*env) -> GetObjectClass(env, packet);

    jfieldID fidAddress_from = (*env) -> GetFieldID(env, udp_packet_class, "address", "Ljava/net/InetAddress;");
    jfieldID fidBuf          = (*env) -> GetFieldID(env, udp_packet_class, "buf", "[B");
    jfieldID fidPort_from    = (*env) -> GetFieldID(env, udp_packet_class, "port", "I");
	
	jfieldID fidAddress_to = (*env) -> GetFieldID(env, udp_packet_class, "toAddr", "Ljava/net/InetAddress;");
    jfieldID fidPort_to    = (*env) -> GetFieldID(env, udp_packet_class, "toPort", "I");

    jclass inet_address_class = (*env) -> FindClass(env, "java/net/InetAddress");
    jmethodID inet_addr_getByName = (*env) -> GetStaticMethodID(env, inet_address_class, "getByName", "(Ljava/lang/String;)Ljava/net/InetAddress;");
	
    jstring host_from = (*env) -> NewStringUTF(env, inet_ntoa(rxinfo.sin_from.sin_addr));
	jstring host_to   = (*env) -> NewStringUTF(env, inet_ntoa(rxinfo.sin_to.sin_addr));
    jobject inet_address_from = (*env) -> CallStaticObjectMethod(env, inet_address_class, inet_addr_getByName, host_from);
	jobject inet_address_to   = (*env) -> CallStaticObjectMethod(env, inet_address_class, inet_addr_getByName, host_to  );

    jbyteArray message = (*env) -> NewByteArray(env, (int)bufLen);
    (*env) -> SetByteArrayRegion(env, message, 0, (int)bufLen,  buf);
    (*env) -> SetObjectField(    env, packet,  fidAddress_from, inet_address_from);
	(*env) -> SetObjectField(    env, packet,  fidAddress_to,   inet_address_to);
    (*env) -> SetObjectField(    env, packet,  fidBuf,          message);
    (*env) -> SetIntField(       env, packet,  fidPort_from,    ntohs(rxinfo.sin_from.sin_port));
	(*env) -> SetIntField(       env, packet,  fidPort_to,      ntohs(rxinfo.sin_to.sin_port));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_mcastJoinC(JNIEnv *env, jobject obj, jint channId, jint ipAddr){
    struct in_addr interface_ip;
    memset(&interface_ip, 0, sizeof(interface_ip));
#ifdef _WIN32
    interface_ip.S_un.S_addr = (int)ipAddr;
#else
    interface_ip.s_addr = (int)ipAddr;
#endif
    printf("Try to dbl_mcast_join() channelId = %d, mcastAddr = %d\n", channId, ipAddr);
    DBL_Safe(dbl_mcast_join((*channels[channId]), &interface_ip, NULL));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_mcastLeaveC(JNIEnv *env, jobject obj, jint channId, jint ipAddr){
    struct in_addr interface_ip;
    memset(&interface_ip, 0, sizeof(interface_ip));
#ifdef _WIN32
    interface_ip.S_un.S_addr = (int)ipAddr;
#else
    interface_ip.s_addr = (int)ipAddr;
#endif
    printf("Try to dbl_mcast_leave() channelId = %d, mcastAddr = %d\n", channId, ipAddr);
    DBL_Safe(dbl_mcast_leave((*channels[channId]), &interface_ip));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_deviceGetAttrsC(JNIEnv *env, jobject obj, jint devId, jobject attrs){
    jclass device_attr_class = (*env) -> GetObjectClass(env, attrs);

    jfieldID fidrecvqFilterMode = (*env) -> GetFieldID(env, device_attr_class, "recvqFilterMode", "I");
    jfieldID fidrecvqSize       = (*env) -> GetFieldID(env, device_attr_class, "recvqSize",       "I");
    jfieldID fidhwTimestamping  = (*env) -> GetFieldID(env, device_attr_class, "hwTimestamping",  "I");
    
    struct dbl_device_attrs attrs_c;
    printf("C: Try to dbl_device_get_attrs()\n");
    DBL_Safe(dbl_device_get_attrs((*devices[devId]), &attrs_c));

    (*env) -> SetIntField(env, attrs, fidrecvqFilterMode, (jint)attrs_c.recvq_filter_mode);
    (*env) -> SetIntField(env, attrs, fidrecvqSize,       (jint)attrs_c.recvq_size       );
    (*env) -> SetIntField(   env, attrs, fidhwTimestamping,  (jint)attrs_c.hw_timestamping  );
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_deviceSetAttrsC(JNIEnv *env, jobject obj, jint devId, jint recvqFilterMode, jint recvqSize, jint hwTimestamping){
    struct dbl_device_attrs attrs_c;
    memset(&attrs_c, 0, sizeof(attrs_c));
    attrs_c.recvq_filter_mode = recvqFilterMode;
    attrs_c.recvq_size = recvqSize;
    attrs_c.hw_timestamping = hwTimestamping;
    printf("C: Try to dbl_device_set_attrs()\n");
    DBL_Safe(dbl_device_set_attrs((*devices[devId]), &attrs_c));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_deviceEnableC(JNIEnv *env, jobject obj, jint devId){
    printf("C: Try to dbl_device_enalbe()\n");
    DBL_Safe(dbl_device_enable((*devices[devId])));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_shutdownC(JNIEnv *env, jobject obj, jint devId){
    printf("C: Try to dbl_shutdown()\n");
    //DBL_Safe(dbl_shutdown((*devices[devId]), 0));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_unbindC(JNIEnv *env, jobject obj, jint channId){
    printf("C: Try to dbl_unbind()\n");
    DBL_Safe(dbl_unbind((*channels[channId])));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_closeC(JNIEnv *env, jobject obj, jint devId){
    printf("C: Try to dbl_close()\n");
    DBL_Safe(dbl_close((*devices[devId])));
}

JNIEXPORT void JNICALL Java_com_sock_udp_DBLUDPSocket_sendDisconnectC(JNIEnv *env, jobject obj, jint handleId){
    printf("C: Try to dbl_send_disconnect()\n");
    DBL_Safe(dbl_send_disconnect((*send_handles[(int)handleId])));
}
