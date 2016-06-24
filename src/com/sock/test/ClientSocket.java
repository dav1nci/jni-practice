package com.sock.test;

import com.sock.udp.UDPPacket;
import com.sock.udp.UDPSocket;

import java.net.InetSocketAddress;
import java.util.concurrent.Callable;

/**
 * Created by stdima on 23.06.16.
 */
public class ClientSocket implements Callable<String> {

    private UDPSocket server;
    private String message;

    public ClientSocket(UDPSocket server, String message) {
        this.server = server;
        this.message = message;
    }

    @Override
    public String call() throws Exception {
        for (int i = 0; i < 5; ++i){
            //System.out.println("sending message");
            server.send(new UDPPacket(this.message.getBytes(), message.length(), new InetSocketAddress("127.0.0.1", 8888)));
            //Thread.sleep(1000);
        }
        return "Client ended";
    }
}
