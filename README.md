###my socket in C + Java
######This code works well on linux

Run the following commands
```
make
make mj
```
to clean all run 
```
make clean
```
test connect() mecthod with following console command
```
sudo sendip -p ipv4 -is 127.0.0.1 -p udp -us 50403 -ud 8888 -d "Hello" -v 127.0.0.1
```
