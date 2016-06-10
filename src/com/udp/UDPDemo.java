package com.udp;

import java.nio.charset.StandardCharsets;

/**
 * Created by stdima on 09.06.16.
 */
public class UDPDemo {
    static{
        System.loadLibrary("udp");
    }

    public static native byte[] sendMessage(byte[] message);

    public static void main(String[] args){
        String message = "Hello my name is Dima, whai is yours?";
        message.getBytes();
        System.out.println("Sending message");
        byte[] response = sendMessage(message.getBytes());
        System.out.println(new String(response, StandardCharsets.UTF_8));
    }
}

