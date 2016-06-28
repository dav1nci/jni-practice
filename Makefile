CSDIR = c_src/
JSDIR = src/com/sock/
LIBDIR = lib/
CLASSPATH = com.sock

all: bin/com/app/nativecalls/NativeCallsDemo.class

bin/com/app/nativecalls/NativeCallsDemo.class:
	mkdir bin
	# javac -d bin/ $(JSDIR)Application.java $(JSDIR)udp/UDPPacket.java $(JSDIR)udp/UDPSocket.java $(JSDIR)test/ClientSocket.java $(JSDIR)test/ServerSocket.java
	find -name "*.java" > source.txt
	javac -d bin/ @source.txt
	rm source.txt
	javah -d $(CSDIR) -classpath ./bin com.sock.udp.KernelUDPSocket
	javah -d $(CSDIR) -classpath ./bin com.sock.udp.DBLUDPSocket
	echo Main-Class: com.sock.Application > bin/MANIFEST.MF

win:
	gcc c_src/socket_win.c -o lib/udp.dll -I"C:\Program Files\Java\jdk1.8.0_91\include" -I"C:\Program Files\Java\jdk1.8.0_91\include\win32"	-shared -l"ws2_32"
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application

lin:
	gcc c_src/kernel_socket.c -Wl,--trace -o lib/libkernel_udp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -shared -fPIC
	gcc c_src/dbl_socket.c -o lib/libdbl_udp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -I/opt/dbl/include -L/opt/dbl/lib -ldbl -Wl,-rpath=/opt/dbl/lib/ -shared -fPIC
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application

clean:
	rm -r bin/
	rm c_src/com_sock_udp_KernelUDPSocket.h
	rm c_src/com_sock_udp_DBLUDPSocket.h
	rm lib/libdbl_udp.so
	rm lib/libkernel_udp.so
	rm lib/libudp.so
	rm UDP.jar

clean-win:
	rmdir bin /s /q
	del c_src\com_sock_udp_UDPSocket.h
	rmdir lib /s /q

jar:
	jar cvfm UDP.jar bin/MANIFEST.MF -C bin/ . lib/

run:
	java -Djava.library.path=lib/ -jar UDP.jar
