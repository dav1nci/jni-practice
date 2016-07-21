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
	gcc c_src/dbl_udp_socket.c -o lib/dbl_udp.dll -I"C:\Program Files\Java\jdk1.8.0_91\include" -I"C:\Program Files\Java\jdk1.8.0_91\include\win32" -I"C:\DBL_Myri-10G\include" -L"C:\DBL_Myri-10G\lib" -ldbl -Wl,-rpath="C:\DBL_Myri-10G\lib" -shared -l"ws2_32"
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application 10.115.66.128 10.115.66.128 8888 3 server 100

lin:
	gcc c_src/kernel_udp_socket.c -o lib/libkernel_udp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -shared -fPIC
	gcc c_src/kernel_tcp_socket.c -o lib/libkernel_tcp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -shared -fPIC
	gcc c_src/dbl_udp_socket.c -o lib/libdbl_udp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -I/opt/dbl/include -L/opt/dbl/lib -ldbl -Wl,-rpath=/opt/dbl/lib/ -shared -fPIC
	gcc c_src/dbl_tcp_socket.c -o lib/libdbl_tcp.so -I/usr/lib/jvm/java-8-oracle/include/ -I/usr/lib/jvm/java-8-oracle/include/linux/ -I/opt/dbl/include -L/opt/dbl/lib -ldbl -Wl,-rpath=/opt/dbl/lib/ -shared -fPIC
	java -Djava.library.path=lib/ -classpath bin/ com.sock.Application 10.115.66.133 10.115.66.133 8888 2 server 100

clean:
	rm -r bin/
	rm c_src/com_sock_udp_KernelUDPSocket.h
	rm c_src/com_sock_udp_DBLUDPSocket.h
	rm lib/libdbl_udp.so
	rm lib/libkernel_udp.so
	rm UDP.jar

clean-win:
	rmdir bin /s /q
	del c_src\com_sock_udp_KernelUDPSocket.h
	del c_src\com_sock_udp_DBLUDPSocket.h
	del lib\dbl_udp.dll
	del lib\kernel_udp.dll
	del UDP.jar

jar:
	echo Class-Path: UDP.jar >> bin/MANIFEST.MF
	echo Main-Class: com.sock.Application >> bin/MANIFEST.MF
	jar cvfm UDP.jar bin/MANIFEST.MF -C bin/ . lib/

run:
	java -Djava.library.path=lib/ -jar UDP.jar 10.115.66.185 10.115.55.185 8888 2 100

ex:
	gcc dbl_pingpong.c -o ex.exe -I"C:\DBL_Myri-10G\include" -I"C:\DBL_Myri-10G\share\doc\examples" -L"C:\DBL_Myri-10G\lib" -ldbl -Wl,-rpath="C:\DBL_Myri-10G\lib"
