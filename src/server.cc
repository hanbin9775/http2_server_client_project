#include <nghttp2/asio_http2_server.h>
#include <iostream>

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

int main(int argc, char *argv[]) {
  boost::system::error_code ec;
  boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);

  tls.use_private_key_file("key/server.key", boost::asio::ssl::context::pem);
  tls.use_certificate_chain_file("key/server.crt");

  configure_tls_context_easy(ec, tls);

  http2 server;

  //cache specific object
  server.handle("/", [](const request &req, const response &res) {
    boost::system::error_code ec;
    auto push = res.push(ec, "GET", "css/style.css");
    push->write_head(200);
    push->end(file_generator("css/style.css"));

    res.write_head(200);
    res.end(file_generator("../index.html"));
  });


  server.handle("/style.css", [](const request &req, const response &res) {
    res.write_head(200);
    res.end(file_generator("css/style.css"));
  });

  if (server.listen_and_serve(ec, tls, "localhost", "3000")) {
    std::cerr << "error: " << ec.message() << std::endl;
  }
}