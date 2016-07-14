package com.sock.udp;

/**
 * Created by stdima on 14.07.16.
 */
public class DeviceAttributes {
    private int recvqFilterMode;
    private int recvqSize;
    private int hwTimestamping;
    private int reserved_1;

    public int getRecvqFilterMode() {
        return recvqFilterMode;
    }

    public int getRecvqSize() {
        return recvqSize;
    }

    public int getHwTimestamping() {
        return hwTimestamping;
    }
}
