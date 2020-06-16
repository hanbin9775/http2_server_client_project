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

    auto req = sess.submit(ec, "GET", "http://localhost:3000");

    req->on_response([&sess](const response &res) {
      std::cerr << "response received!" << std::endl;
      res.on_data([&sess](const uint8_t *data, std::size_t len) {
        std::cerr.write(reinterpret_cast<const char *>(data), len);
        std::cerr << std::endl;
      });
    });

    req->on_push([](const request &push) {
      std::cerr << "push request received!" << std::endl;
      push.on_response([](const response &res) {
        std::cerr << "push response received!" << std::endl;
        res.on_data([](const uint8_t *data, std::size_t len) {
          std::cerr.write(reinterpret_cast<const char *>(data), len);
          std::cerr << std::endl;
        });
      });
    });
  });

  sess.on_error([](const boost::system::error_code &ec) {
    std::cerr << "error: " << ec.message() << std::endl;
  });

  io_service.run();
}