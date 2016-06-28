CSDIR = c_src/
JSDIR = src/com/sock/
LIBDIR = lib/
CLASSPATH = com.sock

all: bin/com/app/nativecalls/NativeCallsDemo.class

bin/com/app/nativecalls/NativeCallsDemo.class:
	mkdir bin
	echo Main-Class: com.sock.Application > bin/MANIFEST.MF
	mkdir lib
	javac -d bin/ $(JSDIR)Application.java $(JSDIR)udp/UDPPacket.java $(JSDIR)udp/UDPSocket.java $(JSDIR)test/ClientSocket.java $(JSDIR)test/ServerSocket.java
	javah -d $(CSDIR) -classpath ./bin com.sock.udp.UDPSocket

win:
	gcc c_src/socket.c -o lib/udp.dll -I"C:\Program Files\Java\jdk1.8.0_91\include" -I"C:\Program Files\Java\jdk1.8.0_91\include\win32"	-shared -l"ws2_32"
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application

lin:
	gcc c_src/socket.c -o lib/libudp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -shared -fPIC
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application

clean:
	rm -r bin/
	rm c_src/com_sock_udp_UDPSocket.h
	rm -r lib/
	rm UDP.jar

clean-win:
	rmdir bin /s /q
	del c_src\com_sock_udp_UDPSocket.h
	rmdir lib /s /q

jar:
	jar cvfm UDP.jar bin/MANIFEST.MF -C bin/ bin/* lib/

run:
	java -Djava.library.path=lib/ -jar UDP.jar
