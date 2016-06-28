package com.sock;

import com.sock.test.ClientSocket;
import com.sock.test.ServerSocket;
import com.sock.udp.UDPPacket;
import com.sock.udp.UDPSocket;

import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.util.concurrent.*;

public class Application {

    public static void main(String[] args) throws UnknownHostException {
        //testSocket();
        System.out.println("-D parameter: " + System.getProperty("java.library.path"));
        try {
            concurrentTest();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static void concurrentTest() throws InterruptedException {
        ExecutorService pool = Executors.newFixedThreadPool(3);
        UDPSocket server = new UDPSocket();
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
        UDPSocket s1 = new UDPSocket();
        UDPSocket s2 = new UDPSocket();

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
}
