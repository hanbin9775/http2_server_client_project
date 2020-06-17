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
2. make key directory and create key file and cert file.

>   mkdir key
>   cd key
>   openssl genrsa -out server.key 2048
>   openssl req -new -x509 -sha256 -key server.key -out server.crt -out server.crt -days 365

3. g++ class/Subscriber.cc -o server.o server.cc -lnghttp2_asio -lboost_system -lssl -lcrypto -lpthread 
4. ./server.o
5.  g++ -o client.o client.cc -lnghttp2_asio -lboost_system -lssl -lcrypto -lpthread
6. ./client.o
7. Keep working!