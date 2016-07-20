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
        socket.bind(new InetSocketAddress(socketAddr, port));
        socket.listen(backlog);
        KernelTCPSocket newConnection = socket.accept();
        String message = "mess from socket";
        socket.send(newConnection, message.getBytes(), flag);

        return null;
    }
}
