package com.sock.udp;

import java.net.*;
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
    protected boolean reuseAddr = false;

    public static AbstractUDPSocket createInstance(SocketAddress address, int flag){
        if (DBLUDPSocket.checkInit())
            return new DBLUDPSocket(address, flag);
        else
            return new KernelUDPSocket();
    }

    public static AbstractUDPSocket createInstance(SocketAddress address, int flag, int port){
        if (DBLUDPSocket.checkInit())
            return new DBLUDPSocket(address, flag);
        else
            return new KernelUDPSocket(port);
    }

    abstract public void send(UDPPacket packet);
    abstract public void bind(SocketAddress addr) throws Exception;
    abstract public void connect(SocketAddress addr);
    abstract public void connect(InetAddress address, int port);
    abstract public void disconnect();
    abstract public void close();
    abstract public void receive(UDPPacket packet) throws Exception;
    abstract public void setReuseAddress(boolean on);
    abstract public void joinGroup(InetAddress mcastaddr, InetAddress interfaceAdr) throws Exception;
    abstract public void leaveGroup(InetAddress mcastaddr, InetAddress interfaceAdr);



    public InetAddress getInetAddress() {
        if (isConnected())
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
        if (isConnected())
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

    public boolean getReuseAddress(){
        return this.reuseAddr;
    }



    public static int hostToInt(SocketAddress address) {
        return ByteBuffer.wrap(((InetSocketAddress) address).getAddress().getAddress())
                .order(ByteOrder.LITTLE_ENDIAN)
                .getInt();
    }

    public static int hostToInt(InetAddress address) {
        return ByteBuffer.wrap(address.getAddress()).order(ByteOrder.LITTLE_ENDIAN).getInt();
    }

    public static String hostToString(int host) {
        ByteBuffer b = ByteBuffer.allocate(4);
        b.order(ByteOrder.LITTLE_ENDIAN);
        b.putInt(host);
        byte[] hostBytes = b.array();
        InetAddress client = null;
        try {
            client = InetAddress.getByAddress(hostBytes);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        return client.getHostAddress();
    }


}
