package com.sock;

import com.sock.udp.UDPPacket;
import com.sock.udp.UDPSocket;
import java.net.UnknownHostException;

public class Application {
    public static void main(String[] args) throws UnknownHostException {
        testSocket();
    }

    public static void testSocket(){
        UDPSocket s1 = new UDPSocket();
        UDPSocket s2 = new UDPSocket();

        String message = "Hello me name is Dima!";
        UDPPacket packet = new UDPPacket("127.0.0.1", 8888, message.getBytes(), message.length());
        int bufLen = 100;
        UDPPacket responce = new UDPPacket(new byte[bufLen], bufLen);
        s2.bind(8888);
        s1.send(packet);
        s2.receive(responce);
        for (byte i : responce.getMessage())
            System.out.print((char)i);
        System.out.println();
    }
}
