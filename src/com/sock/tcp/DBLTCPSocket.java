package com.sock.tcp;

import com.sock.udp.DBLUDPSocket;

/**
 * Created by stdima on 18.07.16.
 */
public class DBLTCPSocket extends DBLUDPSocket{

    public DBLTCPSocket(String address, int flag) throws Exception {
        super(address, flag);
    }

    public DBLTCPSocket() {
    }

    public void tcpSend(byte[] buf, int flag) {
        this.tcpSendC(this.channelId, buf, buf.length, flag);
    }

    public void tcpSend(DBLTCPSocket socket, byte[] buf, int flag) {
        this.tcpSendC(socket.channelId, buf, buf.length, flag);
    }

    public DBLTCPSocket tcpAccept() {
        DBLTCPSocket newConnection = new DBLTCPSocket();
        this.tcpAcceptC(this.channelId, newConnection);
        return newConnection;
    }

    public void tcpListen() {
        this.tcpListenC(this.channelId);
    }

    public byte[] tcpReceive(int recvMode, int bufLen, DBLReceiveInfo info) {
        return this.tcpReceiveC(this.channelId, recvMode, bufLen, info);
    }

    public byte[] tcpReceive(DBLTCPSocket socket, int recvMode, int bufLen, DBLReceiveInfo info) {
        return this.tcpReceiveC(socket.channelId, recvMode, bufLen, info);
    }

    public DBLReceiveInfo[] tcpReceiveMsg(int recvMode, int recvMax) {
        return this.tcpReceiveMsgC(this.deviceId, recvMode, recvMax);
    }

    public int tcpPoll(DBLTCPSocket[] sockets, int arrayLen, int timeout) {
        int[] channels = new int[arrayLen];
        for (int i = 0; i < arrayLen; ++i){
            channels[i] = sockets[i].channelId;
        }
        return this.tcpPollC(channels, arrayLen, timeout);
    }

    public int tcpGetChannelOptions(int level, int optName) {
        return this.getChannelOptionsC(this.channelId, level, optName);
    }

    public void tcpSetChannelOptions(int level, int optName, int optVal) {
        this.setChannelOptionsC(this.channelId, level, optName, optVal);
    }

    public int tcpGetChannelType() {
        return this.getChannelTypeC(this.channelId);
    }

    private native int tcpSendC(int channId, byte[] buf, int bufLen, int flag);
    private native void tcpAcceptC(int channId, DBLTCPSocket newSocket);
    private native void tcpListenC(int channId);
    private native byte[] tcpReceiveC(int channId, int rcvMode, int bufLen, DBLReceiveInfo rcvInfo);
    private native DBLReceiveInfo[] tcpReceiveMsgC(int devId, int rcvMode, int rcvMax);
    private native int tcpPollC(int[] channels, int arrayLen, int timeout);
    private native int getChannelOptionsC(int channId, int level, int optName);
    private native void setChannelOptionsC(int channId, int level, int optName, int optVal);
    private native int getChannelTypeC(int channId);
}
