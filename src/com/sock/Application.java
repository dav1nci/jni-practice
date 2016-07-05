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
        String local, remote;
        if (args.length == 2){
            local = args[0];
            remote = args[1];
        } else {
            System.out.println("You forgot to enter local and remote ip addresses!");
            return;
        }
        //testSocket();
        testDBLSocket(local, remote);
//        try {
//            concurrentTest();
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        }
    }

    public static void concurrentTest() throws InterruptedException {
        ExecutorService pool = Executors.newFixedThreadPool(3);
        KernelUDPSocket server = new KernelUDPSocket();
        server.bind(new InetSocketAddress(8888));
        Callable<String> serverTask = new ServerSocket(server);
        Callable<String> clientTask1 = new ClientSocket("Hello from client1");
        Callable<String> clientTask2 = new ClientSocket("Hello from client2");
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

    public static void testSocket(){
        KernelUDPSocket s1 = new KernelUDPSocket();
        KernelUDPSocket s2 = new KernelUDPSocket();

        String message = "Hello me name is Dima!";
        UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), new InetSocketAddress("127.0.0.1", 8888));

        int bufLen = 100;
        UDPPacket response = new UDPPacket(new byte[bufLen], bufLen);
        s2.bind(new InetSocketAddress(8889));
        //s2.connect(new InetSocketAddress("127.0.0.1", 50403));
        s1.send(packet);
        s2.receive(response);
        System.out.print("Java: ");
        for (byte i : response.getMessage())
            System.out.print((char)i);
        System.out.println();
    }

    public static void testDBLSocket(String local, String remote){
        DBLUDPSocket server = new DBLUDPSocket(new InetSocketAddress(local, 1)); // i don't use port in this version
        DBLUDPSocket client = new DBLUDPSocket(new InetSocketAddress(remote, 1));

        server.bind(new InetSocketAddress(8888));

        String message = "Hello me name is Dima!";
        int bufLen = 100;
        UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), new InetSocketAddress(local, 8888));
        UDPPacket response = new UDPPacket(new byte[bufLen], bufLen);
        client.send(packet);
        server.receive(response);
        System.out.print("Java: ");
        for (byte i : response.getMessage())
            System.out.print((char)i);
        System.out.println();
    }
}
