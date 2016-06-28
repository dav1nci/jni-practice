package com.sock.udp;

import java.net.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by stdima on 14.06.16.
 */
public class KernelUDPSocket extends AbstractUDPSocket{

    static {
        System.loadLibrary("udp");
    }
    private int socketId;

    public KernelUDPSocket() {
        this.socketId = createSocketC();
    }

    public void send(UDPPacket packet){
        sendC(this.socketId, packet.getMessage(), packet.getBufLen(), packet.getHost(), packet.getPort());
    }

    public void bind(SocketAddress addr){
        this.port = ((InetSocketAddress)addr).getPort();
        this.address = ((InetSocketAddress) addr).getAddress();//!!!!!!!! NUllPointer
        this.bound = true;
        bindC(this.socketId, ((InetSocketAddress) addr).getPort());
    }

    public void connect(SocketAddress addr){
        int host = ByteBuffer.wrap(((InetSocketAddress) addr).getAddress().getAddress())
                .order(ByteOrder.LITTLE_ENDIAN)
                .getInt();
        this.remoteAddress = ((InetSocketAddress) addr).getAddress();
        this.remotePort = ((InetSocketAddress) addr).getPort();
        this.connected = true;
        connectC(this.socketId, host, ((InetSocketAddress) addr).getPort());
    }

    public void connect(InetAddress address, int port){
        this.remoteAddress = address;
        this.remotePort = port;
        this.connected = true;
        connectC(this.socketId, ByteBuffer.wrap(address.getAddress()).order(ByteOrder.LITTLE_ENDIAN).getInt(), port);
    }

    public void close(){
        this.closed = true;
        closeC(this.socketId);
    }

    public void receive(UDPPacket packet){
        receiveC(this.socketId, packet, packet.getBufLen());
    }

    private native int createSocketC();
    private native void sendC(int sockId, byte[] buf, int len, int host, int port);
    private native void bindC(int sockId, int port);
    private native void closeC(int sockId);
    private native void receiveC(int sockId, UDPPacket packet, int buflen);
    private native void connectC(int sockId, int host, int port);

}