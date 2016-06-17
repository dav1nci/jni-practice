package com.sock.udp;

import java.net.InetAddress;
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

    public InetAddress getAddresyName(String name) throws UnknownHostException {
        return InetAddress.getByName(name);
    }

    public void send(UDPPacket packet){
        sendC(this.socketId, packet.getMessage(), packet.getBufLen(), packet.getHost(), packet.getPort());
    }

    public void bind(int port){
        bindC(this.socketId, port);
    }

    public void close(){
        closeC(this.socketId);
    }

    public void receive(UDPPacket packet){
        packet.setMessage(receiveC(this.socketId, packet.getBufLen()));
    }

    private native int createSocketC();
    private native void sendC(int sockId, byte[] buf, int len, String host, int port);
    private native void bindC(int sockId, int port);
    private native void closeC(int sockId);
    private native byte[] receiveC(int sockId, int bufLen);

}
