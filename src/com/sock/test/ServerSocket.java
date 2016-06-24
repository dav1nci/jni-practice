package com.sock.test;

import com.sock.udp.UDPPacket;
import com.sock.udp.UDPSocket;

import java.util.concurrent.Callable;

/**
 * Created by stdima on 23.06.16.
 */
public class ServerSocket implements Callable<String> {

    private UDPSocket server;

    public ServerSocket(UDPSocket server) {
        this.server = server;
    }

    @Override
    public String call() throws Exception {
        UDPPacket buf = new UDPPacket(new byte[512], 512);
//        for (int i = 0; i < 5; ++i){
        while(true){
            server.receive(buf);
            System.out.println("Server: message is \"" + new String(buf.getMessage()) + "\"");
            if (false)
                break;
        }
        server.close();
        return "Server done";
    }
}
