package com.sock.udp;

public class UDPPacket {
    private String host;
    private int port;
    private byte[] message;
    private int bufLen;

    public UDPPacket(String host, int port, byte[] message, int bufLen) {
        this.host = host;
        this.port = port;
        this.message = message;
        this.bufLen = bufLen;
    }

    public UDPPacket(byte[] message, int bufLen) {
        this.message = message;
        this.bufLen = bufLen;
    }

    String getHost() {
        return host;
    }

    int getPort() {
        return port;
    }

    public byte[] getMessage() {
        return message;
    }

    public int getBufLen() {
        return bufLen;
    }

    public void setMessage(byte[] message) {
        this.message = message;
    }
}
