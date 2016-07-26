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
#include "com_sock_tcp_DBLTCPSocket.h"

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

int channels_num = 0;
int devices_num = 0;
int send_handles_num = 0;

//enum dbl_recvmode rmode = DBL_RECV_DEFAULT;

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_init(JNIEnv *env, jclass class){
    printf("Try to dbl_init()\n");
    int res = dbl_init(DBL_VERSION_API);
    if (res != 0) {
        printf("dlb_init() return %d\n", res);
    }
    return res;
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

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_sendC(JNIEnv *env, jobject obj, jint handleId, jbyteArray buf, jint bufLen, jint flag){
    jboolean is_copy;
    char *buf_c = (*env) -> GetByteArrayElements(env, buf, &is_copy);
    printf("C: try to dbl_send() message: %s with send_handles_id = %d\n", buf_c, handleId);
    int res = dbl_send((*send_handles[handleId]), buf_c, (int)bufLen, flag);
    (*env) -> ReleaseByteArrayElements(env, buf, buf_c, JNI_ABORT);
    return res;
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_sendToC(JNIEnv *env, jobject obj, jint channId, jint host, jint port, jbyteArray buf, jint bufLen, jint flag){
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
    return dbl_sendto((*channels[channId]), &remote, buf_c, (int)bufLen, 0);
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

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_receiveFromC(JNIEnv *env, jobject obj, jint devId, jint recvMode, jobject packet, jint bufLen){
    assert((int)devId >= 0);
    char buf[(int)bufLen];
    struct dbl_recv_info rxinfo;
    enum dbl_recvmode rmode = recvMode;
    printf("C: Try to dbl_recvfrom() from dev = %d\n", (int)devId);
    int res = dbl_recvfrom((*devices[(int)devId]), rmode, buf, bufLen, &rxinfo);
    printf("C: After dbl_recvfrom() from dev = %d\n", (int)devId);
    jclass udp_packet_class = (*env) -> GetObjectClass(env, packet);

    jfieldID fidAddress_from = (*env) -> GetFieldID(env, udp_packet_class, "address", "I" );
    jfieldID fidBuf          = (*env) -> GetFieldID(env, udp_packet_class, "buf",     "[B");
    jfieldID fidPort_from    = (*env) -> GetFieldID(env, udp_packet_class, "port",    "I" );
    jfieldID fidAddress_to   = (*env) -> GetFieldID(env, udp_packet_class, "toAddr",  "I" );
    jfieldID fidPort_to      = (*env) -> GetFieldID(env, udp_packet_class, "toPort",  "I" );

    jbyteArray message = (*env) -> NewByteArray(env, (int)bufLen);
    (*env) -> SetByteArrayRegion(env, message, 0, (int)bufLen,  buf);
    (*env) -> SetObjectField(    env, packet,  fidBuf,          message);
    (*env) -> SetIntField(       env, packet,  fidAddress_from, rxinfo.sin_from.sin_addr.s_addr);
    (*env) -> SetIntField(       env, packet,  fidAddress_to,   rxinfo.sin_to.sin_addr.s_addr);
    (*env) -> SetIntField(       env, packet,  fidPort_from,    ntohs(rxinfo.sin_from.sin_port));
    (*env) -> SetIntField(       env, packet,  fidPort_to,      ntohs(rxinfo.sin_to.sin_port));
    return res;
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_mcastJoinC(JNIEnv *env, jobject obj, jint channId, jint ipAddr){
    struct in_addr interface_ip;
    memset(&interface_ip, 0, sizeof(interface_ip));
#ifdef _WIN32
    interface_ip.S_un.S_addr = (int)ipAddr;
#else
    interface_ip.s_addr = (int)ipAddr;
#endif
    printf("Try to dbl_mcast_join() channelId = %d, mcastAddr = %d\n", channId, ipAddr);
    return dbl_mcast_join((*channels[channId]), &interface_ip, NULL);
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_mcastLeaveC(JNIEnv *env, jobject obj, jint channId, jint ipAddr){
    struct in_addr interface_ip;
    memset(&interface_ip, 0, sizeof(interface_ip));
#ifdef _WIN32
    interface_ip.S_un.S_addr = (int)ipAddr;
#else
    interface_ip.s_addr = (int)ipAddr;
#endif
    printf("Try to dbl_mcast_leave() channelId = %d, mcastAddr = %d\n", channId, ipAddr);
    return dbl_mcast_leave((*channels[channId]), &interface_ip);
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

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_unbindC(JNIEnv *env, jobject obj, jint channId){
    printf("C: Try to dbl_unbind()\n");
    return dbl_unbind((*channels[channId]));
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_closeC(JNIEnv *env, jobject obj, jint devId){
    printf("C: Try to dbl_close()\n");
    return dbl_close((*devices[devId]));
}

JNIEXPORT jint JNICALL Java_com_sock_udp_DBLUDPSocket_sendDisconnectC(JNIEnv *env, jobject obj, jint handleId){
    printf("C: Try to dbl_send_disconnect()\n");
    return dbl_send_disconnect((*send_handles[(int)handleId]));
}

//=========================TCP PART=======================================
#if 1

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpSendC(JNIEnv *env, jobject obj, jint channId, jbyteArray buf, jint bufLen, jint flag) {
    jboolean is_copy;
    char *buf_c = (*env) -> GetByteArrayElements(env, buf, &is_copy);
    int sendedBytes = 0;
    DBL_Safe(dbl_ext_send((*channels[channId]), buf_c, bufLen, flag, &sendedBytes));
    (*env) -> ReleaseByteArrayElements(env, buf, buf_c, JNI_ABORT);
    return sendedBytes;
}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpAcceptC(JNIEnv *env, jobject obj, jint channId, jobject newSocket) {
    struct sockaddr_in new_socket;
    channels = (dbl_channel_t **) realloc(channels, (channels_num + 1) * sizeof(dbl_channel_t *));
    channels[channels_num] = (dbl_channel_t *) malloc(sizeof(dbl_channel_t));
    channels_num++;
    int sock_len = sizeof(new_socket);
    int res = dbl_ext_accept((*channels[channId]), (struct sockaddr *) &new_socket, &sock_len, NULL, channels[channels_num - 1]);

    jclass DBLTCPSocket_class = (*env) -> GetObjectClass(env, newSocket);

    jfieldID fidAddress   = (*env) -> GetFieldID(env, DBLTCPSocket_class, "address",   "I");
    jfieldID fidPort      = (*env) -> GetFieldID(env, DBLTCPSocket_class, "port",      "I");
    jfieldID fidChannelId = (*env) -> GetFieldID(env, DBLTCPSocket_class, "channelId", "I");

    (*env) -> SetIntField(env, newSocket, fidChannelId, (channels_num - 1)             );
    (*env) -> SetIntField(env, newSocket, fidPort,      ntohs(new_socket.sin_port)     );
#ifdef _WIN32
    (*env) -> SetIntField(env, newSocket, fidAddress,   new_socket.sin_addr.S_un.S_addr);
#else
    (*env) -> SetIntField(env, newSocket, fidAddress,   new_socket.sin_addr.s_addr     );
#endif
    return res;
}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpListenC(JNIEnv *env, jobject obj, jint channId) {
    return dbl_ext_listen((*channels[channId]));
}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpReceiveC(JNIEnv *env, jobject obj, jint channId, jint rcvMode, jbyteArray buf_j, jint bufLen, jobject rcvInfo) {
    enum dbl_recvmode rmode = rcvMode;
    struct dbl_recv_info info;

    jboolean isCopy;
    // Java GC stops here
    jbyte *buf_c = (jbyte *) (*env) -> GetPrimitiveArrayCritical(env, buf_j, &isCopy);
    int res = dbl_ext_recv((*channels[channId]), rmode, buf_c, bufLen, &info);
    // Java GC continue here
    (*env) -> ReleasePrimitiveArrayCritical(env, buf_j, buf_c, JNI_ABORT);

    jclass DBLReceiveInfo_class = (*env) -> GetObjectClass(env, rcvInfo);

    jfieldID fidchannelId = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "channelId", "I");
    jfieldID fidfrom      = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "from",      "I");
    jfieldID fidto        = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "to",        "I");
    jfieldID fidmsgLen    = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "msgLen",    "I");
    jfieldID fidtimestamp = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "timestamp", "I");

    (*env) -> SetIntField(env, rcvInfo, fidchannelId, channId);
    (*env) -> SetIntField(env, rcvInfo, fidfrom,      info.sin_from.sin_addr.s_addr);
    (*env) -> SetIntField(env, rcvInfo, fidto,        info.sin_to.sin_addr.s_addr);
    (*env) -> SetIntField(env, rcvInfo, fidmsgLen,    info.msg_len);
    (*env) -> SetIntField(env, rcvInfo, fidtimestamp, info.timestamp);

    return res;
}

JNIEXPORT jobjectArray JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpReceiveMsgC(JNIEnv *env, jobject obj, jint devId, jint rcvMode, jint rcvMax) {
    enum dbl_recvmode rmode = rcvMode;
    struct dbl_recv_info **info_array = (struct dbl_recv_info **) malloc(rcvMax * sizeof(struct dbl_recv_info *));
    int i = 0;
    for (i = 0; i < rcvMax; ++i) {
        info_array[i] = (struct dbl_recv_info *) malloc(sizeof(struct dbl_recv_info));
    }
    
    DBL_Safe(dbl_ext_recvmsg((*devices[devId]), rmode, info_array, rcvMax));

    jclass DBLReceiveInfo_class = (*env) -> FindClass(env, "com/sock/tcp/DBLReceiveInfo");

    // Get fields in class DBLReceiveInfo
    jfieldID fidchannelId = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "channelId", "I");
    jfieldID fidfrom      = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "from",      "I");
    jfieldID fidto        = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "to",        "I");
    jfieldID fidmsgLen    = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "msgLen",    "I");
    jfieldID fidtimestamp = (*env) -> GetFieldID(env, DBLReceiveInfo_class, "timestamp", "I");
    
    // Get constructor of class DBLReceiveInfo
    jmethodID DBLReceiveInfo_init = (*env) -> GetMethodID(env, DBLReceiveInfo_class, "<init>", "()V");

    jobjectArray result_array = (*env) -> NewObjectArray(env, rcvMax, DBLReceiveInfo_class, NULL);
    for (i = 0; i < rcvMax; ++i) {
        jobject temp = (*env) -> NewObject(env, DBLReceiveInfo_class, DBLReceiveInfo_init);

        //(*env) -> SetIntField(env, temp, fidchannelId, channId);
        (*env) -> SetIntField(env, temp, fidfrom,      info_array[i] -> sin_from.sin_addr.s_addr);
        (*env) -> SetIntField(env, temp, fidto,        info_array[i] -> sin_to.sin_addr.s_addr);
        (*env) -> SetIntField(env, temp, fidmsgLen,    info_array[i] -> msg_len);
        (*env) -> SetIntField(env, temp, fidtimestamp, info_array[i] -> timestamp);

        (*env) -> SetObjectArrayElement(env, result_array, i, temp);
    }

    // Free resources
    for (i = 0; i < rcvMax; ++i) {
        free(info_array[i]);
    }
    free(info_array);

    return result_array;
}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_tcpPollC(JNIEnv *env, jobject obj, jintArray channels_j, jint arrLen, jint timeout) {
    dbl_channel_t channels_c[1000];
    jint *channels_ind_c = (*env) -> GetIntArrayElements(env, channels_j, NULL);
    int i = 0;
    for (i = 0; i < arrLen; ++i) {
        channels_c[i] = (*channels[channels_ind_c[i]]);
    }
    return dbl_ext_poll(channels_c, arrLen, timeout);
}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_getChannelOptionsC(JNIEnv *env, jobject obj, jint channId, jint level, jint optName) {
    int opt_val;
    int val_size = sizeof(int);
    DBL_Safe(dbl_ext_getchopt((*channels[channId]), level, optName, &opt_val, &val_size));
    return opt_val;
}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_setChannelOptionsC(JNIEnv *env, jobject obj, jint channId, jint level, jint optName, jint optVal) {
    int opt_val_c = optVal;
    int opt_size = sizeof(int);
    return dbl_ext_setchopt((*channels[channId]), level, optName, &opt_val_c, opt_size);
}

JNIEXPORT jint JNICALL Java_com_sock_tcp_DBLTCPSocket_getChannelTypeC(JNIEnv *env, jobject obj, jint channId) {
    return dbl_ext_channel_type((*channels[channId]));
}

#endif
