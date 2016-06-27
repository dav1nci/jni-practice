package com.sock.test;

import com.sock.udp.UDPPacket;
import com.sock.udp.UDPSocket;

import java.net.InetSocketAddress;
import java.util.concurrent.Callable;

/**
 * Created by stdima on 23.06.16.
 */
public class ClientSocket implements Callable<String> {

    private String message;

    public ClientSocket(String message) {
        this.message = message;
    }

    @Override
    public String call() throws Exception {
        UDPSocket client = new UDPSocket();
        UDPPacket response = new UDPPacket(new byte[512], 512);
        for (int i = 0; i < 5; i+=0){
            client.send(new UDPPacket(this.message.getBytes(), message.length(), new InetSocketAddress("127.0.0.1", 8888)));
            client.receive(response);
            System.out.println("In client socket responce is");
            for (byte j : response.getMessage())
                System.out.print((char)j);
            System.out.println();
        }
        client.close();
        return "Client ended";
    }
}
