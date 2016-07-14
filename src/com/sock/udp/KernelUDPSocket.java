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
        sendC(this.socketId, packet.getMessage(), packet.getBufLen(), packet.getHost(), packet.getPort());
    }

    @Override
    public void bind(SocketAddress addr){
        this.port = ((InetSocketAddress)addr).getPort();
        this.address = ((InetSocketAddress) addr).getAddress();
        this.bound = true;
        bindC(
                this.socketId,
                hostToInt(addr),
                ((InetSocketAddress) addr).getPort()
        );
    }

    @Override
    public void connect(SocketAddress addr){
        this.remoteAddress = ((InetSocketAddress) addr).getAddress();
        this.remotePort = ((InetSocketAddress) addr).getPort();
        this.connected = true;
        connectC(this.socketId, hostToInt(addr), ((InetSocketAddress) addr).getPort());
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
        receiveC(this.socketId, packet, packet.getBufLen());
    }

    @Override
    public void setReuseAddress(boolean on) {

    }

    @Override
    public void joinGroup(InetAddress mcastaddr) {

    }

    @Override
    public void joinGroup(SocketAddress mcastaddr, NetworkInterface netIf) throws Exception {
        if (!this.isBound())
            throw new Exception("Socket not bound to a port");
        joinMcastGroupC(this.socketId, hostToInt(mcastaddr), hostToInt(netIf.getInetAddresses().nextElement()));
    }

    @Override
    public void leaveGroup(InetAddress mcastaddr) {

    }

    @Override
    public void leaveGroup(SocketAddress mcastaddr, NetworkInterface netIf) {

    }

    private native int createSocketC();
    private native void sendC(int sockId, byte[] buf, int len, int host, int port);
    private native void bindC(int sockId, int host, int port);
    private native void closeC(int sockId);
    private native void receiveC(int sockId, UDPPacket packet, int buflen);
    private native void connectC(int sockId, int host, int port);
    private native void disconnectC(int sockId);
    private native void joinMcastGroupC(int sockId, int mcastGroup, int _interface);

}