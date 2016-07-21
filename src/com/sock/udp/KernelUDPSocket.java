package com.sock.udp;

import java.util.concurrent.TimeUnit;

/**
 * Created by stdima on 14.06.16.
 */
public class KernelUDPSocket extends AbstractUDPSocket{
    private int socketId;

    static {
        System.loadLibrary("kernel_udp");
    }

    public KernelUDPSocket() {
        this.socketId = createSocketC();
        this.closed = false;
    }

    public KernelUDPSocket(int port) throws Exception {
        this.socketId = createSocketC();
        this.closed = false;
        this.bind(hostToInt("127.0.0.1"), port);
        this.bound = true;
    }

    @Override
    public void send(UDPPacket packet){
        if (isConnected())
            sendC(this.socketId, packet.getMessage(), packet.getBufLen());
        else
            sendToC(this.socketId, packet.getMessage(), packet.getBufLen(), packet.getHost(), packet.getPort());
    }

    @Override
    public void bind(int address, int port){
        bindC(this.socketId, address, port);
        this.bound = true;
    }

    @Override
    public void connect(int address, int port){
        this.remoteAddress = address;
        this.remotePort = port;
        this.connected = true;
        connectC(this.socketId, address, port);
    }

    @Override
    public void disconnect() {
        disconnectC(this.socketId);
    }

    @Override
    public void close(){
        this.closed = true;
        closeC(this.socketId);
    }

    @Override
    public void receive(UDPPacket packet) throws Exception {
        if (isConnected()) {
            receiveC(socketId, packet, packet.getBufLen());
            packet.setAddress(this.getRemoteAddress());
            packet.setPort(this.getPort());
        } else {
            receiveFromC(this.socketId, packet, packet.getBufLen());
        }
    }

    @Override
    public void setReuseAddress(boolean on) {
        int flag = (on)? 1 : 0;
        setReuseAddrC(this.socketId, flag);
    }

    @Override
    public void joinGroup(int mcastaddr, int interfaceAddr) throws Exception {
        if (!this.isBound())
            throw new Exception("Socket not bound to a port");
        else
            joinMcastGroupC(this.socketId, mcastaddr, interfaceAddr);
    }

    @Override
    public void leaveGroup(int mcastaddr, int interfaceAddr) {

    }

    public void setTimeout(int n, TimeUnit unit) throws Exception {
        switch (unit){
            case SECONDS:
                setTimeout(this.socketId, n, 0);
                break;
            case MICROSECONDS:
                setTimeout(this.socketId, n, 1);
                break;
            default:
                throw new Exception("Time may be only SECONDS or MICROSECONDS");
        }
    }

    private native int createSocketC();
    private native void sendToC(int sockId, byte[] buf, int len, int host, int port);
    private native void sendC(int sockId, byte[] buf, int len);
    private native void bindC(int sockId, int host, int port);
    private native void closeC(int sockId);
    private native void receiveC(int sockId, UDPPacket packet, int buflen);
    private native void receiveFromC(int sockId, UDPPacket packet, int bufLen);
    private native void connectC(int sockId, int host, int port);
    private native void disconnectC(int sockId);
    private native void joinMcastGroupC(int sockId, int mcastGroup, int _interface);
    private native void leaveMcastGroup(int sockId, int mcastGroup, int _interface);
    private native void setReuseAddrC(int sockId, int flag);
    private native void setTimeout(int sockId, int n, int flag);

}