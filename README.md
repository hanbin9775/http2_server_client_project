# Http2 Server-Client Application
<br>

## Development Enviroment

>Linux Ubuntu-18.04

<br>

## Requirements

nghttp2 library

[for more detailed information][link]

[link]: https://nghttp2.org/documentation/package_README.html

<br>

## So far... 

0. There's no Makefile yet
1. cd src
2. g++ class/Subscriber.cc -o server.o server.cc -lnghttp2_asio -lboost_system -lssl -lcrypto -lpthread 
3. ./server.o
4.  g++ -o client.o client.cc -lnghttp2_asio -lboost_system -lssl -lcrypto -lpthread
5. ./client.o
6. Keep working!