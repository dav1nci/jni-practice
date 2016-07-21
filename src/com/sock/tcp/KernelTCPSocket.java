package com.sock.tcp;

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
    public static int MSG_CONFIRM = 2048;
    public static int MSG_DONTROUTE = 4;
    public static int MSG_DONTWAIT = 64;
    public static int MSG_EOR = 128;
    public static int MSG_MORE = 32768;
    public static int MSG_NOSIGNAL = 16384;
    public static int MSG_OOB = 1;

    private int address;
    private int port;
    private int remoteAddress;
    private int remotePort;

    private int sockId;

    private boolean bound = false;
    private boolean listen = false;
    private boolean connected = false;
    private List<Integer> connectedSockets;

    public KernelTCPSocket() {
        this.connectedSockets = new ArrayList<>();
        this.sockId = createSocketC();
    }

    public KernelTCPSocket(String address, int port) throws Exception {
        this.connectedSockets = new ArrayList<>();
        this.sockId = createSocketC();
        this.connect(address, port);
    }

    public void bind(String addr, int port) throws Exception {
        bindC(this.sockId, hostToInt(addr), port);
        this.address = hostToInt(addr);
        this.port = port;
        this.bound = true;
    }

    public void listen(int backlog) {
        listenC(this.sockId, backlog);
    }

    public KernelTCPSocket accept() {
        KernelTCPSocket result = new KernelTCPSocket();
        acceptC(this.sockId, result);
        connectedSockets.add(result.getSockId());
        result.remoteAddress = this.address;
        result.remotePort = this.port;
        return result;
    }

    public void connect(String addr, int port) throws Exception {
        connectC(this.sockId, hostToInt(addr), port);
        this.remoteAddress = hostToInt(addr);
        this.remotePort = port;
        this.connected = true;
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

    public byte[] receive(KernelTCPSocket socket, int bufLen, int flag) {
        return receiveC(socket.getSockId(), bufLen, flag);
    }

    public byte[] receive(int bufLen, int flag) {
        return receiveC(this.sockId, bufLen, flag);
    }

    public void close() {
        closeC(this.sockId);
    }

    public void setReceiveTimeout(int sec, long microsec) {
        setTimeoutReceiveC(this.sockId, sec, microsec);
    }

    public void setSendTimeout(int sec, long microsec) {
        setTimeoutSendC(this.sockId, sec, microsec);
    }

    public void setReuseAddr(boolean flag) {
        int val = (flag)? 1 : 0;
        setReuseAddrC(this.sockId, val);
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

    private native int createSocketC();
    private native void bindC(int sockId, int host, int port);
    private native void listenC(int sockId, int backlog);
    private native void acceptC(int sockId, KernelTCPSocket socket);
    private native void connectC(int sockId, int host, int port);
    private native void sendC(int sockId, byte[] buf, int bufLen, int flag);
    private native byte[] receiveC(int sockId, int bufLen, int flag);
    private native void setTimeoutReceiveC(int sockId, int sec, long microsec);
    private native void setTimeoutSendC(int sockId, int sec, long microsec);
    private native void setReuseAddrC(int sockId, int flag);
    private native void closeC(int sockId);
}
