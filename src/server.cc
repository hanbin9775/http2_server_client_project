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
      std::cerr<<"received request body : ";
      std::cerr.write(reinterpret_cast<const char *>(data), len);
      std::cerr<<"\n";
      int cur=0;
      int space;
      std::string str;
      str.assign((char*)data,len);
      //check if request body is empty
      if(str!=""){
        space = str.find(' ', cur);
        sub.setFirstName(str.substr(cur, space-cur));
        cur = space + 1;  
        space = str.find(' ',cur);
        sub.setLastName(str.substr(cur, space-cur));
        cur = space + 1;
        space = str.find(' ',cur);
        sub.setPhoneNumber(str.substr(cur, space-cur));
        cur = space + 1;
        space = str.find(' ',cur);
        sub.setAddressp(str.substr(cur, space-cur));
        v.push_back(sub);
        std::cerr<<"v size : " <<v.size()<< std::endl;
        for(int i=0; i<v.size(); i++){
          std::cerr<<v[i].getFirstName()<<std::endl;
          std::cerr<<v[i].getLastName()<<std::endl;
          std::cerr<<v[i].getPhoneNumber()<<std::endl;
          std::cerr<<v[i].getAddressp()<<std::endl;
        }
      }
    });
    res.end("response_body");
  });

  if (server.listen_and_serve(ec, tls, "localhost", "3000")) {
    std::cerr << "error: " << ec.message() << std::endl;
  }
}