compile with this:
```
gcc c_src/udp.c -o bin/libnative_impl.so -I/usr/lib/jvm/java-8-oracle/include/ -I /usr/lib/jvm/java-8-oracle/include/linux/ -shared -fPIC
```

compile server with this:
```
gcc c_src/udp_server.c -o bin/server
```

run server with this
```
./bin/server
```

run java with this:
```
java -Djava.library.path=bin/ -classpath bin/ com.udp.UDPDemo
```
