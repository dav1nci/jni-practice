package com.sock.test;

import com.sock.udp.KernelUDPSocket;
import com.sock.udp.UDPPacket;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.concurrent.Callable;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by stdima on 23.06.16.
 */
public class ServerSocket implements Callable<String> {

    private KernelUDPSocket server;

    public ServerSocket(KernelUDPSocket server) {
        this.server = server;
    }

    @Override
    public String call() throws Exception {
        UDPPacket buf = new UDPPacket(new byte[512], 512);
//        for (int i = 0; i < 5; ++i){
        while(true){
            server.receive(buf);
            System.out.println("Server: message is \"" + new String(buf.getMessage()) + "\"");
            System.out.println("Message comes from " + buf.getHost() + " port: " + buf.getPort());
            ByteBuffer b = ByteBuffer.allocate(4);
            b.order(ByteOrder.LITTLE_ENDIAN);
            b.putInt(buf.getHost());
            byte[] hostBytes = b.array();
            for (byte i : hostBytes){
                System.out.print(i + " ");
            }
            InetAddress client = InetAddress.getByAddress(hostBytes);
            server.send(new UDPPacket(buf.getMessage(),
                    buf.getBufLen(),
                    new InetSocketAddress(client, buf.getPort())));
            if (false)
                break;
        }
        server.close();
        return "Server done";
    }
}
