package com.sock.udp;

import java.net.InetAddress;
import java.net.SocketAddress;

/**
 * Created by stdima on 28.06.16.
 */
public class DBLUDPSocket extends AbstractUDPSocket {

    static {
        System.loadLibrary("dbl_udp");
        init();
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
    public void close() {

    }

    @Override
    public void receive(UDPPacket packet) {

    }

    private static native void init();
    private native int createSocketC();
    private native void sendC(int sockId, byte[] buf, int len, int host, int port);
    private native void bindC(int sockId, int port);
    private native void closeC(int sockId);
    private native void receiveC(int sockId, UDPPacket packet, int buflen);
    private native void connectC(int sockId, int host, int port);
}
