#include <iostream>
#include <nghttp2/asio_http2_client.h>

using boost::asio::ip::tcp;

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::client;

int main(int argc, char *argv[]) {
  boost::system::error_code ec;
  boost::asio::io_service io_service;

  boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);
  tls.set_default_verify_paths();
  // disabled to make development easier...
  // tls_ctx.set_verify_mode(boost::asio::ssl::verify_peer);
  configure_tls_context(ec, tls);

  // connect to localhost:3000
  session sess(io_service, tls, "localhost", "3000");

  sess.on_connect([&sess](tcp::resolver::iterator endpoint_it) {
    boost::system::error_code ec;

    //Command Line UI
    std::cout<<"**************************************"<<std::endl;
    std::cout<<"*       Chatting Applicaion          *"<<std::endl;
    std::cout<<"**************************************"<<std::endl<<std::endl;

    std::cout<<"0. quit"<<std::endl;
    std::cout<<"1. register"<<std::endl;
    std::cout<<"2. chat"<<std::endl;
    std::cout<<"3. Ask for location"<<std::endl<<std::endl;
    std::cout<<"Input : ";
    
    int input;
    std::cin >> input;
    
    switch(input){
      //if input == 1
      case 1:{
        std::string temp;
        std::string request_body ="";
        std::cout<<"First Name : ";
        std::cin>> temp;
        request_body+=temp+" ";
        std::cout<<"Last Name : ";
        std::cin>> temp;
        request_body+=temp+" ";
        std::cout<<"PhoneNumber : ";
        std::cin>> temp;
        request_body+=temp+" ";
        std::cout<<"Address : ";
        std::cin>> temp;
        request_body+=temp+" ";
        std::cerr<<request_body<<std::endl;
        
        auto register_req = sess.submit(ec, "POST", "http://localhost:3000/register",request_body);
        
        register_req->on_response([&sess](const response &res) {
          //shutdown for now
          res.on_data([](const uint8_t *data, std::size_t len) {
            std::cerr.write(reinterpret_cast<const char *>(data), len);
            std::cerr << std::endl;
          });
          sess.shutdown();
        });
        break;}
      case 3:{
        std::cerr<<"search location of ... [insert name] : ";
        std::string name;
        std::cin >> name;
        auto search_req = sess.submit(ec, "GET", "http://localhost:3000/search",name);
         search_req->on_response([&sess](const response &res) {
          //shutdown for now
          res.on_data([](const uint8_t *data, std::size_t len) {
            std::cerr.write(reinterpret_cast<const char *>(data), len);
            std::cerr << std::endl;
          });
          sess.shutdown();
        });
        break;}
      case 4: {
        auto req = sess.submit(ec, "GET", "http://localhost:3000/");
         req->on_response([&sess](const response &res) {
          //shutdown for now
          res.on_data([](const uint8_t *data, std::size_t len) {
            std::cerr.write(reinterpret_cast<const char *>(data), len);
            std::cerr << std::endl;
          });
          sess.shutdown();
        });
        break;
      }
      default:{
        std::cerr<<"wrong input. terminate client"<<std::endl;
        sess.shutdown();
        break;}
    }
  });

  sess.on_error([](const boost::system::error_code &ec) {
    std::cerr << "error: " << ec.message() << std::endl;
  });

  io_service.run();
}