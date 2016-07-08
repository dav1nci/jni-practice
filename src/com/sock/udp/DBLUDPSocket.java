package com.sock.udp;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by stdima on 28.06.16.
 */
public class DBLUDPSocket extends AbstractUDPSocket {
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

    static {
        System.loadLibrary("dbl_udp");
        init();
    }

    public DBLUDPSocket(SocketAddress address, int flag) {
        this.deviceId = createDeviceC(hostToInt(address), flag);
    }

    public void send(UDPPacket packet, int flag) {
        sendC(this.sendHandleId, packet.getMessage(), packet.getBufLen(), flag);
    }

    public void sendTo(UDPPacket packet, int flag){
        sendToC(this.channelId, packet.getHost(), packet.getPort(), packet.getMessage(), packet.getBufLen(), flag);
    }

    public void sendConnect(SocketAddress addr, int flag, int ttl){ // DBL sdk no flag supported for this method
        this.port = ((InetSocketAddress)addr).getPort();
        this.address = ((InetSocketAddress) addr).getAddress();//!!!!!!!! NUllPointer
        sendConnectC(this.channelId, hostToInt(addr), ((InetSocketAddress)addr).getPort(), flag, ttl);
    }

    public void bind(SocketAddress addr, int flag) {
        this.port = ((InetSocketAddress)addr).getPort();
        this.address = ((InetSocketAddress) addr).getAddress();//!!!!!!!! NUllPointer
        this.bound = true;
        this.channelId = bindC(this.deviceId, flag, ((InetSocketAddress) addr).getPort());
    }

    @Override
    public void send(UDPPacket packet) {

    }

    @Override
    public void bind(SocketAddress addr) {

    }

    @Override
    public void connect(SocketAddress addr) {

    }

    @Override
    public void connect(InetAddress address, int port) {

    }

    @Override
    public void receive(UDPPacket packet) {

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
    }

    public void receive(UDPPacket packet, int receiveMode) {
        receiveFromC(this.deviceId, receiveMode, packet, packet.getBufLen());
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
    private native void closeC(int devId);
}
