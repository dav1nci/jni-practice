package com.sock.udp;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.UnknownHostException;

/**
 * Created by stdima on 14.06.16.
 */
public class UDPSocket {

    static {
        System.loadLibrary("udp");
    }
    private int socketId;

    public UDPSocket() {
        this.socketId = createSocketC();
    }

    public void send(UDPPacket packet){

        sendC(this.socketId, packet.getMessage(), packet.getBufLen(), packet.getHost(), packet.getPort());
    }

    public void bind(SocketAddress addr){
        bindC(this.socketId, ((InetSocketAddress) addr).getPort());
    }

    public void close(){
        closeC(this.socketId);
    }

    public void receive(UDPPacket packet){
        packet.setMessage(receiveC(this.socketId, packet.getBufLen()));
    }

    private native int createSocketC();
    private native void sendC(int sockId, byte[] buf, int len, int host, int port);
    private native void bindC(int sockId, int port);
    private native void closeC(int sockId);
    private native byte[] receiveC(int sockId, int bufLen);

}
