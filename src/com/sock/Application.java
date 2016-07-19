package com.sock;

import com.sock.test.ClientSocket;
import com.sock.test.ServerSocket;
import com.sock.udp.DBLUDPSocket;
import com.sock.udp.UDPPacket;
import com.sock.udp.KernelUDPSocket;
import com.sock.udp.DeviceAttributes;

import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.concurrent.*;

public class Application {

    public static void main(String[] args) throws Exception {
        String server, client;
		int port;
        String mcasAddr = "224.5.5.5";
		server = args[0];
		client= args[1];
		port = Integer.parseInt(args[2]);

        switch (args[3]){
            case "1":
                testSocket(server, client, port);
                break;
            case "2":
				String serverOrClient = args[4];
				if (serverOrClient.equals("server")) {
					startDblServer(server, client, port);
				} else if (serverOrClient.equals("client")) {
					startDblClient(server, client, port);
				}
                break;
            case "3":
                try {
                        concurrentTest(server, port);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                break;
            case "4":
                testMulticastKernel(client, mcasAddr, port);
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
        s1.connect(new InetSocketAddress(server, port));
        s1.send(packet);
        s2.receive(response);
        System.out.print("Java: ");
        for (byte i : response.getMessage())
            System.out.print((char)i);
        System.out.println();
    }

    public static void testDBLSocket(String serverAddr, String clientAddr, int port) throws Exception {
        System.out.println("Test DBL socket");
		DBLUDPSocket server = new DBLUDPSocket(new InetSocketAddress(serverAddr, 0), DBLUDPSocket.DBL_OPEN_THREADSAFE); // 0 in port parameter ignored
		DBLUDPSocket server2 = new DBLUDPSocket(new InetSocketAddress(serverAddr, 0), DBLUDPSocket.DBL_OPEN_THREADSAFE); // 0 in port parameter ignored
		
		DeviceAttributes attrs = new DeviceAttributes();
		server.deviceGetAttributes(attrs);
		System.out.println("filter = " + attrs.getRecvqFilterMode() 
		+ " resvqSize = " + attrs.getRecvqSize() 
		+ " hwTimestamping = " + attrs.getHwTimestamping());
		
        //DBLUDPSocket client = new DBLUDPSocket(new InetSocketAddress(clientAddr, 0), DBLUDPSocket.DBL_OPEN_THREADSAFE);
        try {
            server.setBindFlag(DBLUDPSocket.DBL_BIND_REUSEADDR);
            server.bindAddr(new InetSocketAddress(serverAddr, port));
			server2.setBindFlag(DBLUDPSocket.DBL_BIND_REUSEADDR);
            server2.bindAddr(new InetSocketAddress(serverAddr, port + 1));
            //client.bind(new InetSocketAddress(port));
        } catch (Exception e) {
            e.printStackTrace();
        }
        String message = "Hello me name is Dima!";
        int bufLen = message.length();
        UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), new InetSocketAddress(serverAddr, port));
        UDPPacket response = new UDPPacket(new byte[bufLen], bufLen);
		System.out.println("Java: try to client.connect()");
		//client.connect(new InetSocketAddress(serverAddr, port));
		//System.out.println("Java: try to client.send()");
        //client.send(packet);
		server2.setRecvMode(DBLUDPSocket.DBL_RECV_DEFAULT);
        server2.receive(response);
        System.out.print("Java: ");
        for (byte i : response.getMessage())
            System.out.print((char)i);
        System.out.println();
		System.out.println("Message comes from " + response.getHost() + ":" + response.getPort());
		System.out.println("Message comes to " + response.getToAddr() + ":" + response.getToPort());
		
    }
	
	public static void startDblServer(String serverAddr, String clientAddr, int port) throws Exception {
		DBLUDPSocket server = new DBLUDPSocket(new InetSocketAddress(serverAddr, 0), DBLUDPSocket.DBL_OPEN_THREADSAFE); // 0 in port parameter ignored
		
		DeviceAttributes attrs = new DeviceAttributes();
		server.deviceGetAttributes(attrs);
		System.out.println("filter = " + attrs.getRecvqFilterMode() 
		+ " resvqSize = " + attrs.getRecvqSize() 
		+ " hwTimestamping = " + attrs.getHwTimestamping());
		
        try {
            server.setBindFlag(DBLUDPSocket.DBL_BIND_REUSEADDR);
            server.bindAddr(new InetSocketAddress(serverAddr, port));
        } catch (Exception e) {
            e.printStackTrace();
        }
		
        String message = "Hello me name is Dima!";
        UDPPacket response = new UDPPacket(new byte[100], 100);
		
		server.setRecvMode(DBLUDPSocket.DBL_RECV_DEFAULT);
		
		for (int i = 0; i < 100; ++i){
			System.out.println("Another server iteration");
			server.receive(response);
			System.out.print("Java: received message: ");
			for (byte i : response.getMessage())
				System.out.print((char)i);
			System.out.println();
			System.out.println("Java: Message comes from " + response.getHost() + ":" + response.getPort());
			System.out.println("Java: Message comes to " + response.getToAddr() + ":" + response.getToPort());
			UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), new InetSocketAddress(clientAddr, port));
			server.send(packet);
		}
		
	}
	
	public static void startDblClient(String serverAddr, String clientAddr, int port) throws Exception {
		DBLUDPSocket client = new DBLUDPSocket(new InetSocketAddress(clientAddr, 0), DBLUDPSocket.DBL_OPEN_THREADSAFE);
		DBLUDPSocket client2 = new DBLUDPSocket(new InetSocketAddress(clientAddr, 0), DBLUDPSocket.DBL_OPEN_THREADSAFE);
		DeviceAttributes attrs = new DeviceAttributes();
		client.deviceGetAttributes(attrs);
		System.out.println("filter = " + attrs.getRecvqFilterMode() 
		+ " resvqSize = " + attrs.getRecvqSize() 
		+ " hwTimestamping = " + attrs.getHwTimestamping());        
        try {
            client.bind(new InetSocketAddress(port));
			client2.bind(new InetSocketAddress(port + 1));
        } catch (Exception e) {
            e.printStackTrace();
        }
		
		//sending part
        String message = "Hello me name is Dima!";
        int bufLen = message.length();
        UDPPacket packet = new UDPPacket(message.getBytes(), message.length(), new InetSocketAddress(serverAddr, port));
		System.out.println("Java: try to client.connect()");
		client.connect(new InetSocketAddress(serverAddr, port));
		System.out.println("Java: try to client.send()");
        client.send(packet);
		System.out.println("Java: message sended.");
		
		//receiving part
		client.setRecvMode(DBLUDPSocket.DBL_RECV_DEFAULT);
		UDPPacket response = new UDPPacket(new byte[100], 100);
		client.receive(response);
		System.out.print("Java: Responce is ");
        for (byte i : response.getMessage())
            System.out.print((char)i);
		System.out.println();
	}

    public static void testMulticastKernel(String clientAddr, String mcastAddr, int port) {
        KernelUDPSocket client1 = new KernelUDPSocket();
        //KernelUDPSocket client2 = new KernelUDPSocket();
        client1.setReuseAddress(true);
        //client2.setReuseAddress(true);
        client1.bind(new InetSocketAddress(clientAddr, port));
        //client2.bind(new InetSocketAddress(clientAddr, port));
        System.out.println("mcast is " + mcastAddr + "interface is " + clientAddr);
        try {
            client1.joinGroup(InetAddress.getByName(mcastAddr), InetAddress.getByName(clientAddr));
            //client2.joinGroup(InetAddress.getByName(mcastAddr), InetAddress.getByName(clientAddr));
        } catch (Exception e) {
            e.printStackTrace();
        }
        UDPPacket response1= new UDPPacket(new byte[50], 50);
        //UDPPacket response2 = new UDPPacket(new byte[50], 50);
        client1.receive(response1);
        //client2.receive(response2);
    }

    public static void testMulticastDBL(){

    }
}
