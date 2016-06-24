CSDIR = c_src/
JSDIR = src/com/sock/
LIBDIR = lib/

all: bin/com/app/nativecalls/NativeCallsDemo.class

bin/com/app/nativecalls/NativeCallsDemo.class:
	mkdir bin
	mkdir lib
	javac -d bin/ $(JSDIR)Application.java $(JSDIR)udp/UDPPacket.java $(JSDIR)udp/UDPSocket.java $(JSDIR)test/ClientSocket.java $(JSDIR)test/ServerSocket.java
	javah -d $(CSDIR) -classpath ./bin com.sock.udp.UDPSocket

mj:
	gcc c_src/socket.c -o lib/libudp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -shared -fPIC
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application

clean:
	rm -r bin/
	rm c_src/com_sock_udp_UDPSocket.h
	rm -r lib/

