###my socket in C + Java
######This code works well on linux

Run the following commands
```
make
make lin #for linux
make win #for Windows
```
to clean all run 
```
make clean #for linux
make clean-win #for Windows
```
to make executalbe jar run 
```
make jar
```
to run it run
```
make run
```
test connect() mecthod with following console command
```
sudo sendip -p ipv4 -is 127.0.0.1 -p udp -us 50403 -ud 8888 -d "Hello" -v 127.0.0.1
```
