package com.sock.test.tcp_test;

import com.sock.tcp.KernelTCPSocket;

import java.util.concurrent.Callable;

/**
 * Created by stdima on 20.07.16.
 */
public class KernelClient implements Callable<Void> {
    private KernelTCPSocket socket;

    //public KernelClient(KernelTCPSocket socket)
    @Override
    public Void call() throws Exception {

        return null;
    }
}
