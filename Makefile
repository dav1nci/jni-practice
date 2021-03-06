CSDIR = c_src/
JSDIR = src/com/sock/
LIBDIR = lib/
CLASSPATH = com.sock

all: bin/com/app/nativecalls/NativeCallsDemo.class

#
# Linux:
# find -name "*.java" > source.txt
#
# Windows:
# dir *.java /b/s > source.txt
#

bin/com/app/nativecalls/NativeCallsDemo.class:
	mkdir bin
	javac -d bin/ @source.txt
	javah -d $(CSDIR) -classpath ./bin com.sock.udp.KernelUDPSocket
	javah -d $(CSDIR) -classpath ./bin com.sock.udp.DBLUDPSocket
	javah -d $(CSDIR) -classpath ./bin com.sock.tcp.KernelTCPSocket
	javah -d $(CSDIR) -classpath ./bin com.sock.tcp.DBLTCPSocket

win:
	gcc c_src/kernel_udp_socket.c -o lib/kernel_udp.dll -I"C:\Program Files\Java\jdk1.8.0_91\include" -I"C:\Program Files\Java\jdk1.8.0_91\include\win32" -shared -l"ws2_32"
	gcc c_src/kernel_tcp_socket.c -o lib/kernel_tcp.dll -I"C:\Program Files\Java\jdk1.8.0_91\include" -I"C:\Program Files\Java\jdk1.8.0_91\include\win32" -shared -l"ws2_32"
	gcc c_src/dbl_tcp_udp_socket.c -o lib/dbl_tcp_udp.dll -I"C:\Program Files\Java\jdk1.8.0_91\include" -I"C:\Program Files\Java\jdk1.8.0_91\include\win32" -I"C:\DBL_Myri-10G\include" -L"C:\DBL_Myri-10G\lib" -l"dbl" -l"dbltcp" -Wl,-rpath="C:\DBL_Myri-10G\lib" -shared -l"ws2_32"
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application 192.168.0.105 10.115.66.128 8888 6 server 100

lin:
	gcc c_src/kernel_udp_socket.c -o lib/libkernel_udp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -shared -fPIC
	gcc c_src/kernel_tcp_socket.c -o lib/libkernel_tcp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -shared -fPIC
	gcc c_src/dbl_tcp_udp_socket.c -o lib/libdbl_tcp_udp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -I/opt/dbl/include -L/opt/dbl/lib -ldbl -Wl,-rpath=/opt/dbl/lib/ -shared -fPIC
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application 192.168.0.102 192.168.0.102 8888 5

clean:
	rm -r bin/
	rm c_src/com_sock_udp_KernelUDPSocket.h
	rm c_src/com_sock_udp_DBLUDPSocket.h
	rm c_src/com_sock_tcp_DBLTCPSocket.h
	rm c_src/com_sock_tcp_KernelTCPSocket.h
	rm lib/libdbl.so
	rm lib/libkernel_udp.so
	rm lib/libkernel_tcp.so
	rm UDP.jar

clean-win:
	rmdir bin /s /q
	del c_src\com_sock_udp_KernelUDPSocket.h
	del c_src\com_sock_udp_DBLUDPSocket.h
	del c_src\com_sock_tcp_DBLTCPSocket.h
	del c_src\com_sock_tcp_KernelTCPSocket.h
	del lib\dbl.dll
	del lib\kernel_udp.dll
	del lib\kernel_tcp.dll
	del UDP.jar

jar:
	echo Class-Path: UDP.jar >> bin/MANIFEST.MF
	echo Main-Class: com.sock.Application >> bin/MANIFEST.MF
	jar cvfm UDP.jar bin/MANIFEST.MF -C bin/ . lib/

run:
	java -Djava.library.path=lib/ -jar UDP.jar 10.115.66.185 10.115.55.185 8888 2 100

exe:
	gcc dbltcp_pingpong.c -o dbltcp_pingpong_source.exe -I"C:\Program Files\Java\jdk1.8.0_91\include" -I"C:\Program Files\Java\jdk1.8.0_91\include\win32" -I"C:\DBL_Myri-10G\include" -L"C:\DBL_Myri-10G\lib" -l"dbl" -l"dbltcp" -Wl,-rpath="C:\DBL_Myri-10G\lib" -l"ws2_32"