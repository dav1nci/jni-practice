package com.sock.udp;

import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class UDPPacket {

    private InetAddress address;
    private int port;
    private byte[] buf;
    private int bufLen;

    /**
     * Constructs a datagram packet for sending packets of length
     * bufLen to the specified port number on the specified host.
     *
     * @param   buf      the packet data.
     * @param   bufLen   the packet data length.
     * @param   address  the destination address.
     * @param   port     the destination port number.
     */
    public UDPPacket(byte[] buf, int bufLen, InetAddress address, int port) {
        this.buf = buf;
        this.bufLen = bufLen;
        this.address = address;
        this.port = port;
    }

    /**
     * Constructs a datagram packet for sending packets of length
     * bufLen to the specified port number on the specified host.
     *
     * @param   buf      the packet data.
     * @param   bufLen   the packet data length.
     * @param   address  the destination socket address.
     */
    public UDPPacket(byte[] buf, int bufLen, SocketAddress address) {
        this.bufLen = bufLen;
        this.buf = buf;
        InetSocketAddress addr = (InetSocketAddress) address;
        this.address = addr.getAddress();
        this.port = addr.getPort();
    }

    /**
     * Constructs a UDPPacket for receiving packets of
     * length bufLen
     *
     * @param   buf      buffer for holding the incoming datagram.
     * @param   bufLen   the number of bytes to read.
     */
    public UDPPacket(byte[] buf, int bufLen) {
        this.bufLen = bufLen;
        this.buf = buf;
    }

    public int getHost() {
        return ByteBuffer.wrap(this.address.getAddress()).order(ByteOrder.LITTLE_ENDIAN).getInt();
    }

    public int getPort() {
        return port;
    }

    public byte[] getMessage() {
        return buf;
    }

    public int getBufLen() {
        return bufLen;
    }

    public void setMessage(byte[] buf) {
        this.buf = buf;
    }

    public void setAddress(InetAddress address) {
        this.address = address;
    }

    public void setPort(int port) {
        this.port = port;
    }
}