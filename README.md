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
1. build library

>   ./configure --enable-asio-lib

>   make

>   make install

2. cd src
3. make key directory and create key file and cert file.

>   mkdir key

>   cd key

>   openssl genrsa -out server.key 2048

>   openssl req -new -x509 -sha256 -key server.key -out server.crt -out server.crt -days 365

4. compile server source file and run.

>   g++ class/Subscriber.cc -o server.o server.cc -lnghttp2_asio -lboost_system -lssl -lcrypto -lpthread 

>   ./server.o

5.  compile client source file and run.

>   g++ -o client.o client.cc -lnghttp2_asio -lboost_system -lssl -lcrypto -lpthread

>   ./client.o