#include <nghttp2/asio_http2_server.h>
#include <iostream>
#include <vector>
#include "class/Subscriber.h"

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

#define MAX_SIZE 5

int main(int argc, char *argv[]) {
  boost::system::error_code ec;
  boost::asio::ssl::context tls(boost::asio::ssl::context::sslv23);

  tls.use_private_key_file("key/server.key", boost::asio::ssl::context::pem);
  tls.use_certificate_chain_file("key/server.crt");

  configure_tls_context_easy(ec, tls);

  http2 server;

  std::vector<Subscriber> v;

  Subscriber sub;

  //Register
  server.handle("/register", [&sub, &v](const request &req, const response &res) {
    res.write_head(200);
    req.on_data([&sub, &v](const uint8_t *data, std::size_t len){
      //request body parser
      int cur=0;
      int space;
      std::string str;
      str = (char *)data;
      std::cerr<<str;
      space = str.find(' ', cur);
      std::cerr<<space<<std::endl;
      sub.setFirstName(str.substr(cur, space-cur));
      cur = space + 1;  
      space = str.find(' ',cur);
      std::cerr<<space<<std::endl;
      sub.setLastName(str.substr(cur, space-cur));
      cur = space + 1;
      space = str.find(' ',cur);
      std::cerr<<space<<std::endl;
      sub.setPhoneNumber(str.substr(cur, space-cur));
      cur = space + 1;
      space = str.find(' ',cur);
      std::cerr<<space<<std::endl;
      sub.setAddressp(str.substr(cur, space-cur));
      v.push_back(sub);
      for(int i=0; i<v.size(); i++){
        std::cerr<<v[i].getFirstName()<<std::endl;
        std::cerr<<v[i].getLastName()<<std::endl;
        std::cerr<<v[i].getPhoneNumber()<<std::endl;
        std::cerr<<v[i].getAddressp()<<std::endl;
      }
    });

    res.end("complete");
  });

  if (server.listen_and_serve(ec, tls, "localhost", "3000")) {
    std::cerr << "error: " << ec.message() << std::endl;
  }
}