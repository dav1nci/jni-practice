package com.sock.udp;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.List;
import java.util.Map;

/**
 * Created by stdima on 28.06.16.
 */
public class DBLUDPSocket extends AbstractUDPSocket {
    private static Map<String, Integer> ipInUse;
    // dbl_open() flags
    public static int DBL_OPEN_THREADSAFE = 1;
    public static int DBL_OPEN_DISABLED  = 2;
    public static int DBL_OPEN_HW_TIMESTAMPING = 4;

    // dbl_bind() flags
    public static int DBL_BIND_REUSEADDR = 2;
    public static int DBL_BIND_DUP_TO_KERNEL = 4;
    public static int DBL_BIND_NO_UNICAST = 8;
    public static int DBL_BIND_BROADCAST = 10;

    // dbl_sendto() and dbl_send() flags
    public static int DBL_NONBLOCK = 4;


    private int deviceId;
    private int channelId;
    private int sendHandleId;

    private int sendFlag = 0;
    private int bindFlag = -1;
    private int recvMode = -1;

    static {
        System.loadLibrary("dbl_udp");
        init();
    }

    public DBLUDPSocket(SocketAddress address, int flag) {
        if (!ipInUse.containsKey(((InetSocketAddress) address).getHostString())){
            this.deviceId = createDeviceC(hostToInt(address), flag);
            ipInUse.put(((InetSocketAddress) address).getHostString(), this.deviceId);
        } else {
            this.deviceId = ipInUse.get(((InetSocketAddress) address).getHostString());
        }
        this.closed = false;
    }

    public void send(UDPPacket packet, int flag) {
        sendC(this.sendHandleId, packet.getMessage(), packet.getBufLen(), flag);
    }

    public void sendTo(UDPPacket packet, int flag){
        sendToC(this.channelId, packet.getHost(), packet.getPort(), packet.getMessage(), packet.getBufLen(), flag);
    }

    @Override
    public void send(UDPPacket packet) {
        if (connected){
            sendC(this.sendHandleId, packet.getMessage(), packet.getBufLen(), this.sendFlag);
        } else {
            sendToC(this.channelId, packet.getHost(), packet.getPort(), packet.getMessage(), packet.getBufLen(), this.sendFlag);
        }
    }

    @Override
    public void bind(SocketAddress addr) throws Exception {
        if (bound)
            throw new Exception("Socket already bound to port " + this.getLocalPort());
        if (bindFlag != -1) {
            this.port = ((InetSocketAddress) addr).getPort();
            this.bound = true;
            this.channelId = bindC(this.deviceId, this.bindFlag, ((InetSocketAddress) addr).getPort());
        }else
            throw new Exception("Bind flag is not specified");
    }

    @Override
    public void connect(SocketAddress addr) {
        this.sendHandleId = sendConnectC(this.channelId, AbstractUDPSocket.hostToInt(addr), ((InetSocketAddress)addr).getPort(), 0, 0);
        this.connected = true;
    }

    @Override
    public void connect(InetAddress address, int port) {
        this.connect(new InetSocketAddress(address.getHostAddress(), port));
    }

    @Override
    public void disconnect() {
        sendDisconnect(this.sendHandleId);
    }

    @Override
    public void receive(UDPPacket packet) throws Exception {
        if (recvMode > -1) {
            receiveFromC(this.deviceId, this.recvMode, packet, packet.getBufLen());
        } else
            throw new Exception("Receive mod not specified!");
    }

    @Override
    public void close() {
        this.closed = true;
        closeC(this.deviceId);
    }

    public void shutdown(){
        shutdownC(this.deviceId);
    }

    public void unbind(){
        unbindC(this.channelId);
        this.bound = false;
    }

    public void receive(UDPPacket packet, int receiveMode) {
        receiveFromC(this.deviceId, receiveMode, packet, packet.getBufLen());
    }

    public void setBindFlag(int bindFlag) {
        this.bindFlag = bindFlag;
    }

    public void setRecvMode(int recvMode) {
        this.recvMode = recvMode;
    }

    private static native void init();
    private native int createDeviceC(int host, int flag);
    private native void sendC(int handleId, byte[] buf, int bufLen, int flag);
    private native void sendToC(int channId, int host, int port, byte[] buf, int bufLen, int flag);
    private native int sendConnectC(int channId, int host, int port, int flag, int ttl);
    private native int bindC(int devId, int flag, int port);
    private native void receiveFromC(int devId, int recvMode, UDPPacket packet, int bufLen);
    private native void shutdownC(int devId);
    private native void unbindC(int channId);
    private native void sendDisconnect(int handleId);
    private native void closeC(int devId);
}
