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
    while(input!=0){
      switch(input){
        case 0:
          sess.shutdown();
          break;
        case 1:
          //POST /register
          auto register_req = sess.submit(ec, "POST", "http://localhost:3000/register");
          register_req->on_response([&sess](const response &res) {
            std::cerr << "registeration complete!" << std::endl;
            res.on_data([&sess](const uint8_t *data, std::size_t len) {
              std::cerr.write(reinterpret_cast<const char *>(data), len);
            });
            //shutdown for now
            sess.shutdown();
          });
          input=0;
          break;
      }
    }
  });

  sess.on_error([](const boost::system::error_code &ec) {
    std::cerr << "error: " << ec.message() << std::endl;
  });

  io_service.run();
}