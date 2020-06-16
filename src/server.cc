#include <nghttp2/asio_http2_server.h>
#include <iostream>
#include "class/Subscriber.h"

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

int main(int argc, char *argv[]) {
  boost::system::error_code ec;
  boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);

  tls.use_private_key_file("key/server.key", boost::asio::ssl::context::pem);
  tls.use_certificate_chain_file("key/server.crt");

  configure_tls_context_easy(ec, tls);

  http2 server;
  Subscriber sub;

  //Register
  server.handle("/register", [&sub](const request &req, const response &res) {
    res.write_head(200);
    sub.setFirstName("HanBin");
    sub.setLastName("Kim");
    sub.setPhoneNumber("01012349775");
    sub.setAddressp("S.Korea");
    res.end("Username : "+sub.getFirstName()+" is registered!");
  });

  if (server.listen_and_serve(ec, tls, "localhost", "3000")) {
    std::cerr << "error: " << ec.message() << std::endl;
  }
}