package com.sock.tcp;

import com.sock.udp.UDPPacket;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;
import java.util.jar.Pack200;

/**
 * Created by stdima on 18.07.16.
 */
public class KernelTCPSocket {
    static {
        System.loadLibrary("kernel_tcp");
    }

    private int sockId;

    private boolean bound = false;
    private boolean listen = false;
    private boolean connected = false;
    private List<Integer> connectedSockets;

    public KernelTCPSocket() {
        this.connectedSockets = new ArrayList<>();
        this.sockId = createSocketC();
    }

    public KernelTCPSocket(InetAddress address, int port) {
        this.connectedSockets = new ArrayList<>();
        this.sockId = createSocketC();
        this.connect(new InetSocketAddress(address, port));
    }

    public void bind(SocketAddress addr) {
        bindC(this.sockId, KernelTCPSocket.hostToInt(addr), ((InetSocketAddress) addr).getPort());
        this.bound = true;
    }

    public void listen(int backlog) {
        listenC(this.sockId, backlog);
    }

    public KernelTCPSocket accept() {
        KernelTCPSocket result = new KernelTCPSocket();
        acceptC(this.sockId, result);
        connectedSockets.add(result.getSockId());
        return result;
    }

    public void connect(SocketAddress addr) {
        connectC(this.sockId, hostToInt(addr), ((InetSocketAddress) addr).getPort());
    }

    public void send(KernelTCPSocket socket, byte[] buf, int flag) throws Exception {
        if (connectedSockets.contains(socket.getSockId())) {
            sendC(socket.getSockId(), buf, buf.length, flag);
        } else {
            throw new Exception("socket is not connected");
        }
    }

    public void send(byte buf[], int flag) throws Exception {
        if (isConnected()) {
            sendC(this.sockId, buf, buf.length, flag);
        } else {
            throw new Exception("this socket is not connected");
        }
    }

    public void receive(KernelTCPSocket socket, byte[] buf, int flag) {
        buf = receiveC(this.sockId, flag);
    }

    public void receive(byte buf[], int flag) {
        buf = receiveC(this.sockId, flag);
    }

    public void close() {
        closeC(this.sockId);
    }

    public void setReceiveTimeout(int sec, long microsec) {
        setTimeoutReceiveC(sec, microsec);
    }

    public void setSendTimeout(int sec, long microsec) {
        setTimeoutSendC(sec, microsec);
    }

    public void setReuseAddr(boolean flag) {
        int val = (flag)? 1 : 0;
        setReuseAddrC(val);
    }

    public boolean isBound() {
        return bound;
    }

    public boolean isListen() {
        return listen;
    }

    public boolean isConnected() {
        return connected;
    }

    public int getSockId() {
        return sockId;
    }

    public static int hostToInt(SocketAddress address) {
        return ByteBuffer.wrap(((InetSocketAddress) address).getAddress().getAddress())
                .order(ByteOrder.LITTLE_ENDIAN)
                .getInt();
    }

    public static int hostToInt(InetAddress address) {
        return ByteBuffer.wrap(address.getAddress()).order(ByteOrder.LITTLE_ENDIAN).getInt();
    }

    private native int createSocketC();
    private native void bindC(int sockId, int host, int port);
    private native void listenC(int sockId, int backlog);
    private native void acceptC(int sockId, KernelTCPSocket socket);
    private native void connectC(int sockId, int host, int port);
    private native void sendC(int sockId, byte[] buf, int bufLen, int flag);
    private native byte[] receiveC(int sockId, int flag);
    private native void setTimeoutReceiveC(int sec, long microsec);
    private native void setTimeoutSendC(int sec, long microsec);
    private native void setReuseAddrC(int flag);
    private native void closeC(int sockId);
}
