package com.sock;

import com.sock.udp.UDPPacket;
import com.sock.udp.UDPSocket;

import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class Application {
    public static void main(String[] args) throws UnknownHostException {
        testSocket();
    }

    public static void testSocket(){
        UDPSocket s1 = new UDPSocket();
        UDPSocket s2 = new UDPSocket();

        String message = "Hello me name is Dima!";
        UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), new InetSocketAddress("127.0.0.1", 8888));

        int bufLen = 100;
        UDPPacket responce = new UDPPacket(new byte[bufLen], bufLen);
        s2.bind(new InetSocketAddress(8888));
        s2.connect(new InetSocketAddress("127.0.0.1", 50403));
        s1.send(packet);
        s2.receive(responce);
        System.out.print("Java: ");
        for (byte i : responce.getMessage())
            System.out.print((char)i);
        System.out.println();
    }
}
