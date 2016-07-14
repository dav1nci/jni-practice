package com.sock.udp;

import java.net.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

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

    public KernelUDPSocket(int port){
        this.socketId = createSocketC();
        this.closed = false;
        this.bind(new InetSocketAddress("127.0.0.1", port));
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
    public void bind(SocketAddress addr){
        bindC(
                this.socketId,
                hostToInt(addr),
                ((InetSocketAddress) addr).getPort()
        );
        this.bound = true;
    }

    @Override
    public void connect(SocketAddress addr){
        connectC(this.socketId, hostToInt(addr), ((InetSocketAddress) addr).getPort());
        this.remoteAddress = ((InetSocketAddress) addr).getAddress();
        this.remotePort = ((InetSocketAddress) addr).getPort();
        this.connected = true;
    }

    @Override
    public void connect(InetAddress address, int port){
        this.remoteAddress = address;
        this.remotePort = port;
        this.connected = true;
        connectC(this.socketId, hostToInt(address), port);
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
    public void receive(UDPPacket packet){
        if (isConnected())
            receiveC(socketId, packet, packet.getBufLen());
        else {
            receiveFromC(this.socketId, packet, packet.getBufLen());
            packet.setAddress(this.getInetAddress());
            packet.setPort(this.getPort());
        }
    }

    @Override
    public void setReuseAddress(boolean on) {
        int flag = (on)? 1 : 0;
        setReuseAddrC(this.socketId, flag);
    }

    @Override
    public void joinGroup(InetAddress mcastaddr, InetAddress interfaceAddr) throws Exception {
        if (!this.isBound())
            throw new Exception("Socket not bound to a port");
        else
            joinMcastGroupC(this.socketId, hostToInt(mcastaddr), hostToInt(interfaceAddr));
    }

    @Override
    public void leaveGroup(InetAddress mcastaddr, InetAddress interfaceAddr) {

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

}