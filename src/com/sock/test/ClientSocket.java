package com.sock.test;

import com.sock.udp.KernelUDPSocket;
import com.sock.udp.UDPPacket;

import java.net.InetSocketAddress;
import java.util.concurrent.Callable;

/**
 * Created by stdima on 23.06.16.
 */
public class ClientSocket implements Callable<String> {

    private String message;
    private String address;

    public ClientSocket(String message, String address) {
        this.message = message;
        this.address = address;
    }

    @Override
    public String call() throws Exception {
        KernelUDPSocket client = new KernelUDPSocket();
        UDPPacket response = new UDPPacket(new byte[512], 512);
        for (int i = 0; i < 5; ++i){
            client.send(new UDPPacket(this.message.getBytes(), message.length(), new InetSocketAddress(address, 8888)));
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
