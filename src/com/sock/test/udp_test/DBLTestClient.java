package com.sock.test.udp_test;

import com.sock.udp.AbstractUDPSocket;
import com.sock.udp.DBLUDPSocket;
import com.sock.udp.UDPPacket;

import java.net.InetSocketAddress;
import java.util.concurrent.Callable;

/**
 * Created by stdima on 19.07.16.
 */
public class DBLTestClient implements Callable<String> {
    private String clientAddr;
    private int port;
    private String serverAddr;
    private String message;
    private int interval;

    public DBLTestClient(String serverAddr, String clientAddr, int port, String message, int interval) {
        this.clientAddr = clientAddr;
        this.port = port;
        this.serverAddr = serverAddr;
        this.message = message;
        this.interval = interval;
    }

    @Override
    public String call() throws Exception {
        DBLUDPSocket client = new DBLUDPSocket(clientAddr, DBLUDPSocket.DBL_OPEN_THREADSAFE);
        try {
            client.bind(0, port);
        } catch (Exception e) {
            e.printStackTrace();
        }

        //sending part
        int bufLen = message.length();
        UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), AbstractUDPSocket.hostToInt(this.serverAddr), 8888);
        UDPPacket response = new UDPPacket(new byte[100], 100);

        System.out.println("Java: try to client.connect()");
        client.connect(AbstractUDPSocket.hostToInt(this.serverAddr), this.port);
        client.setRecvMode(DBLUDPSocket.DBL_RECV_DEFAULT);

        for (int i = 0; i < 30; ++i) {
            System.out.println("Java: try to client.send()");
            client.send(packet);
            System.out.println("Java: message sended.");

            //receiving part
            client.receive(response);
            System.out.print("Java: Responce is ");
            for (byte j : response.getMessage())
                System.out.print((char) j);
            System.out.println();
            Thread.sleep(interval);
        }
        return "Done";
    }
}