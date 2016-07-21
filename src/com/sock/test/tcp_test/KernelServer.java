package com.sock.test.tcp_test;

import com.sock.tcp.KernelTCPSocket;

import java.net.InetSocketAddress;
import java.util.concurrent.Callable;

/**
 * Created by stdima on 20.07.16.
 */
public class KernelServer implements Callable<Void> {
    private KernelTCPSocket socket;
    private String socketAddr;
    private int port;
    private int backlog;
    private int flag;

    public KernelServer(KernelTCPSocket socket, String socketAddr, int port, int backlog, int flag) {
        this.socket = socket;
        this.socketAddr = socketAddr;
        this.port = port;
        this.backlog = backlog;
        this.flag = flag;
    }

    @Override
    public Void call() throws Exception {
        socket.bind(socketAddr, port);
        socket.listen(backlog);
        while (true) {

            System.out.println("Java: try to accept()");
            KernelTCPSocket newConnection = socket.accept();
            String message = "mess from socket";
            System.out.println("Java: server try to send message: " + message);
            socket.send(newConnection, message.getBytes(), flag);
            System.out.println("Java: server try to receive message: ");
            byte[] responce = socket.receive(newConnection, 50, 0);
            System.out.println("Java: server receive message: " + new String(responce));
            //socket.close();
            if (false)
                break;
        }
        return null;
    }
}
