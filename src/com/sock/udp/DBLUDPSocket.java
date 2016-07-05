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

    static {
        System.loadLibrary("dbl_udp");
        init();
    }

    public DBLUDPSocket(SocketAddress address) {
        int host = ByteBuffer.wrap(((InetSocketAddress) address).getAddress().getAddress())
                .order(ByteOrder.LITTLE_ENDIAN)
                .getInt();
        this.socketId = createSocketC(host);
    }

    @Override
    public void send(UDPPacket packet) {
        sendC(this.socketId, packet.getMessage(), packet.getBufLen(), packet.getHost(), packet.getPort());
    }

    @Override
    public void bind(SocketAddress addr) {
        this.port = ((InetSocketAddress)addr).getPort();
        this.address = ((InetSocketAddress) addr).getAddress();//!!!!!!!! NUllPointer
        this.bound = true;
        bindC(this.socketId, ((InetSocketAddress) addr).getPort());
    }

    @Override
    public void connect(SocketAddress addr) {

    }

    @Override
    public void connect(InetAddress address, int port) {

    }

    @Override
    public void close() {
        this.closed = true;
        closeC(this.socketId);
    }

    @Override
    public void receive(UDPPacket packet) {
        receiveC(this.socketId, packet, packet.getBufLen());
    }

    private static native void init();
    private native int createSocketC(int host);
    private native void sendC(int sockId, byte[] buf, int len, int host, int port);
    private native void bindC(int sockId, int port);
    private native void closeC(int sockId);
    private native void receiveC(int sockId, UDPPacket packet, int buflen);
    private native void connectC(int sockId, int host, int port);
}
