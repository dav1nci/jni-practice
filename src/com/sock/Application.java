package com.sock;

import com.sock.test.ClientSocket;
import com.sock.test.ServerSocket;
import com.sock.udp.DBLUDPSocket;
import com.sock.udp.UDPPacket;
import com.sock.udp.KernelUDPSocket;

import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.util.concurrent.*;

public class Application {

    public static void main(String[] args) throws UnknownHostException {
        String server, client;
		int port;
        if (args.length == 3){
            server = args[0];
            client= args[1];
			port = Integer.parseInt(args[2]);
        } else {
            System.out.println("You forgot to enter local and remote ip addresses! and port");
            return;
        }

        switch (args[3]){
            case "1":
                testSocket(server, client, port);
                break;
            case "2":
                testDBLSocket(server, client, port);
                break;
            case "3":
                try {
                        concurrentTest(server, port);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                break;
        }
    }

    public static void concurrentTest(String serverIp, int port) throws InterruptedException {
        ExecutorService pool = Executors.newFixedThreadPool(3);
        KernelUDPSocket server = new KernelUDPSocket();
        server.bind(new InetSocketAddress(serverIp, port));
        Callable<String> serverTask = new ServerSocket(server);
        Callable<String> clientTask1 = new ClientSocket("Hello from client1", serverIp);
        Callable<String> clientTask2 = new ClientSocket("Hello from client2", serverIp);
        Future<String> serverResponse = pool.submit(serverTask);
        Future<String> clientResponse1 = pool.submit(clientTask1);
        //Thread.sleep(200);
        Future<String> clientResponse2 = pool.submit(clientTask2);
        try {
            System.out.println(serverResponse.get());
            System.out.println(clientResponse1.get());
            System.out.println(clientResponse2.get());
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        pool.shutdown();
    }

    public static void testSocket(String server, String client, int port){
        KernelUDPSocket s1 = new KernelUDPSocket();
        KernelUDPSocket s2 = new KernelUDPSocket();

        String message = "Hello me name is Dima!";
        UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), new InetSocketAddress(server, port));

        int bufLen = 100;
        UDPPacket response = new UDPPacket(new byte[bufLen], bufLen);
        s2.bind(new InetSocketAddress(server, port));
        //s2.connect(new InetSocketAddress("127.0.0.1", 50403));
        s1.send(packet);
        s2.receive(response);
        System.out.print("Java: ");
        for (byte i : response.getMessage())
            System.out.print((char)i);
        System.out.println();
    }

    public static void testDBLSocket(String serverAddr, String clientAddr, int port){
        DBLUDPSocket server = new DBLUDPSocket(new InetSocketAddress(serverAddr, 1), DBLUDPSocket.DBL_OPEN_THREADSAFE); // i don't use port in this version
        DBLUDPSocket client = new DBLUDPSocket(new InetSocketAddress(clientAddr, 1), DBLUDPSocket.DBL_OPEN_THREADSAFE);

        server.bind(new InetSocketAddress(port));
		client.bind(new InetSocketAddress(port + 1));

        String message = "Hello me name is Dima!";
        int bufLen = message.length();
        UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), new InetSocketAddress(serverAddr, port));
        UDPPacket response = new UDPPacket(new byte[bufLen], bufLen);
        client.send(packet);
        server.receive(response);
        System.out.print("Java: ");
        for (byte i : response.getMessage())
            System.out.print((char)i);
        System.out.println();
    }
}
