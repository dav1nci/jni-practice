package com.sock.test.tcp_test;

import com.sock.tcp.KernelTCPSocket;

import java.util.concurrent.Callable;

/**
 * Created by stdima on 20.07.16.
 */
public class KernelClient implements Callable<Void> {
    private KernelTCPSocket socket;
    private String serverAddr;
    private int serverPort;
    private int flag;

    public KernelClient(KernelTCPSocket socket, String serverAddr, int serverPort, int flag) {
        this.socket = socket;
        this.serverAddr = serverAddr;
        this.serverPort = serverPort;
        this.flag = flag;
    }

    @Override
    public Void call() throws Exception {
        System.out.println("Java: try to connect");
        socket.connect(serverAddr, serverPort);
        byte[] responce = socket.receive(50, 0);
        System.out.print("Java: client receive message from server: ");
        for (byte i : responce)
            System.out.print((char)i);
        System.out.println();
        String clientMessage = "Thanks";
        socket.send(clientMessage.getBytes(), 0);
        System.out.println("Java: client send \"Thanks\" message.");
        return null;
    }
}
