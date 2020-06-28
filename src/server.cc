#include <nghttp2/asio_http2_server.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

#define MAX_SIZE 5
#define BUF_SIZE 256

int main(int argc, char *argv[]) {
  boost::system::error_code ec;
  boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);

  tls.use_private_key_file("key/server.key", boost::asio::ssl::context::pem);
  tls.use_certificate_chain_file("key/server.crt");

  configure_tls_context_easy(ec, tls);

  http2 server;

  //change this directory to your own root directory
  std::string docroot = "/mnt/c/Users/user/Desktop/Joel/http2_server_client_project";

  server.handle("/subscribers/", [&docroot](const request &req, const response &res) {
    res.write_head(200);
    if(req.method()=="POST"){
      req.on_data([&docroot,&res,&req](const uint8_t *data, std::size_t len){
        std::string str;
        str.assign((char*)data,len);
        //check if request body is empty
        if(str!=""){
          //open file
          auto path = percent_decode(req.uri().path);
          path = docroot + path;
          std::ofstream writeFile;
          writeFile.open(path,std::ios::out);
          if(writeFile.is_open()){
            writeFile << str;
            writeFile.close();
          }
          res.end("registeration complete");
        }
      });
    }
    else if(req.method()=="GET"){
      auto path = docroot+ percent_decode(req.uri().path);
      std::ifstream readFile;
      readFile.open(path, std::ios::in);
      std::string line;
      std::string request_body;
      if(readFile.is_open()){
        while(std::getline(readFile, line)){
          request_body = line.c_str();
        }
      }
      else std::cerr<<"there is no such subscriber"<<std::endl;  
      res.end(request_body);
    }
  });

  if (server.listen_and_serve(ec, tls, "localhost", "3000")) {
    std::cerr << "error: " << ec.message() << std::endl;
  }
}