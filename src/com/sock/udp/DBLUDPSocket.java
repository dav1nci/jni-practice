package com.sock.udp;

import com.sock.tcp.DBLTCPSocket;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by stdima on 28.06.16.
 */
public class DBLUDPSocket extends AbstractUDPSocket {
    private static int initStatus;

    static {
        System.loadLibrary("dbl_tcp_udp");
        System.out.println("Java: library loaded");
        initStatus = init();
    }

    private static Map<String, Integer> ipInUse = new HashMap<>(2); // 2 is random number of amount of devices

    // dbl_open() flags
    public static int DBL_OPEN_THREADSAFE = 1;
    public static int DBL_OPEN_DISABLED  = 2;
    public static int DBL_OPEN_HW_TIMESTAMPING = 4;

    // dbl_bind() flags
    public static int DBL_BIND_REUSEADDR = 2;
    public static int DBL_BIND_DUP_TO_KERNEL = 4;
    public static int DBL_BIND_NO_UNICAST = 8;
    public static int DBL_BIND_BROADCAST = 10;

    // dbl_sendto() and dbl_send() flags
    public static int DBL_NONBLOCK = 4;
	
    public static int DBL_RECV_DEFAULT = 0;

    protected int deviceId;
    protected int channelId;
    protected int sendHandleId;

    private int sendFlag = 0;
    private int bindFlag = -1;
    private int recvMode = -1;
    private int openFlag = -1;


    public DBLUDPSocket(String address, int flag) throws Exception {
        if (!ipInUse.containsKey(address)){
            this.deviceId = createDeviceC(hostToInt(address), flag);
            this.openFlag = flag;
            ipInUse.put((address), this.deviceId);
        } else {
            this.deviceId = ipInUse.get(address);
        }
        this.closed = false;
    }

    public DBLUDPSocket() {
    }

    @Override
    public void send(UDPPacket packet) {
        if (connected){
            sendC(this.sendHandleId, packet.getMessage(), packet.getBufLen(), this.sendFlag);
        } else {
            sendToC(this.channelId, packet.getHost(), packet.getPort(), packet.getMessage(), packet.getBufLen(), this.sendFlag);
        }
    }

    public void bind(int port) throws Exception {
        if (isBound())
            throw new Exception("Socket already bound to port " + this.getLocalPort());
        this.channelId = bindC(this.deviceId, this.bindFlag, port);
        this.port = port;
        this.bound = true;
    }

    public void bindAddr(String addr, int port) throws Exception {
		System.out.println("Java: bindAddr() called");
        if (isBound())
            throw new Exception("Socket already bound to port " + this.getLocalPort());
        this.channelId = bindAddrC(this.deviceId, hostToInt(addr), this.bindFlag, port);
        this.port = port;
        this.bound = true;
    }

    @Override
    public void connect(int address, int port) {
        this.sendHandleId = sendConnectC(this.channelId, address, port, 0, 0);
        this.connected = true;
    }

    @Override
    public void disconnect() {
        sendDisconnectC(this.sendHandleId);
    }

    @Override
    public void receive(UDPPacket packet) throws Exception {
        if (recvMode > -1) {
            receiveFromC(this.deviceId, this.recvMode, packet, packet.getBufLen());
        } else
            throw new Exception("Receive mod not specified!");
    }

    @Override
    public void setReuseAddress(boolean on) {

    }

    @Override
    public void joinGroup(int mcastaddr, int interfaceAddr) {
        mcastJoinC(this.channelId, mcastaddr);
    }

    @Override
    public void leaveGroup(int mcastaddr, int interfaceAddr) {
        mcastLeaveC(this.channelId, mcastaddr);
    }

    @Override
    public void close() {
        this.closed = true;
        closeC(this.deviceId);
    }

    public void deviceEnable() throws Exception {
        if (this.openFlag == DBL_OPEN_DISABLED)
            deviceEnableC(this.deviceId);
        else
            throw new Exception("Device open flag is not a DBL_OPEN_DISABLED");
    }

    public void deviceGetAttributes(DeviceAttributes attrs){
        deviceGetAttrsC(this.deviceId, attrs);
    }

    public void deviceSetAttributes(DeviceAttributes attrs){
        deviceSetAttrsC(this.deviceId, attrs.getRecvqFilterMode(), attrs.getRecvqSize(), attrs.getHwTimestamping());
    }

    public void shutdown(){
        shutdownC(this.deviceId);
    }

    public void unbind(){
        unbindC(this.channelId);
        this.bound = false;
    }

    public void receive(UDPPacket packet, int receiveMode) {
        receiveFromC(this.deviceId, receiveMode, packet, packet.getBufLen());
    }

    public void setBindFlag(int bindFlag) {
        this.bindFlag = bindFlag;
    }

    public void setRecvMode(int recvMode) {
        this.recvMode = recvMode;
    }

    private static native int init();
    private native int createDeviceC(int host, int flag);
    //private native int createDeviceByInterfaceC(String interfaceName, int flag);
    private native int sendC(int handleId, byte[] buf, int bufLen, int flag);
    private native int sendToC(int channId, int host, int port, byte[] buf, int bufLen, int flag);
    private native int sendConnectC(int channId, int host, int port, int flag, int ttl);
    private native int bindC(int devId, int flag, int port);
    private native int bindAddrC(int devId, int interfaceIp, int flag, int port);
    private native int receiveFromC(int devId, int recvMode, UDPPacket packet, int bufLen);

    private native int mcastJoinC(int channId, int ipAddr);
    private native int mcastLeaveC(int channId, int ipAddr);

    private native void deviceGetAttrsC(int devId, DeviceAttributes attrs);
    private native void deviceSetAttrsC(int devId, int recvqFilterMode, int recvqSize, int hwTimestamping);

    private native void deviceEnableC(int devId);

    private native void shutdownC(int devId);
    private native int unbindC(int channId);
    private native int sendDisconnectC(int handleId);
    private native int closeC(int devId);
}
