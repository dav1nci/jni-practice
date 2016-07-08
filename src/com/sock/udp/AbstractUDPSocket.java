package com.sock.udp;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by stdima on 28.06.16.
 */
public abstract class AbstractUDPSocket {

    protected InetAddress remoteAddress;
    protected int remotePort;
    protected InetAddress address;
    protected int port;
    protected boolean bound = false;
    protected boolean closed = false;
    protected boolean connected = false;

    abstract public void send(UDPPacket packet);
    abstract public void bind(SocketAddress addr);
    abstract public void connect(SocketAddress addr);
    abstract public void connect(InetAddress address, int port);
    abstract public void close();
    abstract public void receive(UDPPacket packet);

    public InetAddress getInetAddress() {
        return remoteAddress;
    }

    public InetAddress getLocalSocketAddress() {
        return address;
    }

    public int getLocalPort() {
        return port;
    }

    public boolean isBound() {
        return bound;
    }

    public boolean isClosed() {
        return closed;
    }

    public boolean isConnected() {
        return connected;
    }

    protected int hostToInt(SocketAddress address){
        return ByteBuffer.wrap(((InetSocketAddress) address).getAddress().getAddress())
                .order(ByteOrder.LITTLE_ENDIAN)
                .getInt();
    }
}
