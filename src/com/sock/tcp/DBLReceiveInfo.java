package com.sock.tcp;

/**
 * Created by stdima on 21.07.16.
 */
public class DBLReceiveInfo {
    private int channelId;
    private int from;
    private int to;
    private int msgLen;
    private int timestamp;

    public int getChannelId() {
        return channelId;
    }

    public int getFrom() {
        return from;
    }

    public int getTo() {
        return to;
    }

    public int getMsgLen() {
        return msgLen;
    }

    public int getTimestamp() {
        return timestamp;
    }
}
