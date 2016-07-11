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
    protected boolean closed = true;
    protected boolean connected = false;

    abstract public void send(UDPPacket packet);
    abstract public void bind(SocketAddress addr) throws Exception;
    abstract public void connect(SocketAddress addr);
    abstract public void connect(InetAddress address, int port);
    abstract public void disconnect();
    abstract public void close();
    abstract public void receive(UDPPacket packet) throws Exception;

    public InetAddress getInetAddress() {
        if (connected)
            return remoteAddress;
        return null;
    }

    public InetAddress getLocalSocketAddress() {
        return address;
    }

    public int getLocalPort() {
        if (bound)
            return port;
        else if (closed)
            return -1;
        return 0;
    }

    public int getPort(){
        if (connected)
            return remotePort;
        return -1;
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

    public static int hostToInt(SocketAddress address){
        return ByteBuffer.wrap(((InetSocketAddress) address).getAddress().getAddress())
                .order(ByteOrder.LITTLE_ENDIAN)
                .getInt();
    }


}
