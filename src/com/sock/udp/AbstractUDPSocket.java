package com.sock.udp;

import java.net.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.StringTokenizer;

/**
 * Created by stdima on 28.06.16.
 */
public abstract class AbstractUDPSocket {
    protected int remoteAddress;
    protected int remotePort;
    protected int address;
    protected int port;
    protected boolean bound = false;
    protected boolean closed = true;
    protected boolean connected = false;
    protected boolean reuseAddr = false;

    abstract public void send(UDPPacket packet);
    abstract public void bind(int address, int port) throws Exception;
    abstract public void connect(int address, int port);
    abstract public void disconnect();
    abstract public void close();
    abstract public void receive(UDPPacket packet) throws Exception;
    abstract public void setReuseAddress(boolean on);
    abstract public void joinGroup(int mcastaddr, int interfaceAdr) throws Exception;
    abstract public void leaveGroup(int mcastaddr, int interfaceAdr);



    public int getRemoteAddress() throws Exception {
        if (isConnected())
            return remoteAddress;
        throw new Exception("socket not connected");
    }

    public int getLocalAddress() {
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



    public static int hostToInt(String host) throws Exception {
        if ( host == null || host.length() < 7 || host.length() > 15)
            throw new Exception("Bad ip address argument1");
        String[] ipByStringParts = host.split("\\.");
        if ( ipByStringParts.length != 4)
            throw new Exception("Bad ip address argument2");
        int ipInt = 0;
        for (int i = 3; i >= 0; --i) {
            int ipVal = Integer.parseInt(ipByStringParts[i]);
            if ( ipVal < 0 || ipVal > 255)
                throw new Exception("Bad ip address argument3");
            ipInt = (ipInt << 8) + ipVal;
        }
        return ipInt;
    }

    public static int hostToInt2(String host) throws Exception {
        if ( host == null || host.length() < 7 || host.length() > 15)
            throw new Exception("Bad ip address argument");

        String[] ipByStringParts = host.split("\\.");
        if ( ipByStringParts.length != 4)
            throw new Exception("Bad ip address argument2");
        int ipInt = 0;
        for (int i = 3; i >= 0; --i) {
            int ipVal = Integer.parseInt(ipByStringParts[i]);
            if ( ipVal < 0 || ipVal > 255)
                throw new Exception("Bad ip address argument3");
            ipInt = (ipInt << 8) + ipVal;
        }
        return ipInt;
    }

    public static int hostToInt(SocketAddress address) {
        byte host[] = ((InetSocketAddress) address).getAddress().getAddress();
        return host[3] << 24 | (host[2] & 0xFF) << 16 | (host[1] & 0xFF) << 8 | (host[0] & 0xFF);
    }

    public static int hostToInt(InetAddress address) {
        byte host[] = address.getAddress();
        return host[3] << 24 | (host[2] & 0xFF) << 16 | (host[1] & 0xFF) << 8 | (host[0] & 0xFF);
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
