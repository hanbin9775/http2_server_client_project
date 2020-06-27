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
    res.end("registeration complete");
  });

  
  //Search address
  server.handle("/search", [&v](const request &req, const response &res) {
    res.write_head(200);
    std::string address ="default";
    req.on_data([&v,&address](const uint8_t *data, std::size_t len){
      //check if request body is empty
      std::string name;
      name.assign((char*)data,len);
      for(int i=0; i<v.size(); i++){
        if(name==v[i].getLastName()){
          address = v[i].getAddressp();
        }
      }
    });
    res.end(v[0].getAddressp());
  });

  server.handle("/", [](const request &req, const response &res) {
      auto path = percent_decode(req.uri().path);
      if (!check_path(path)) {
        res.write_head(404);
        res.end();
        return;
      }

      if (path == "/") {
        path = "/subscriber.txt";
      }

      path = "/mnt/c/Users/user/Desktop/Joel/http2_server_client_project" + path;
      auto fd = open(path.c_str(), O_RDONLY);
      if (fd == -1) {
        res.write_head(404);
        res.end();
        std::cerr<<"no such file";
        return;
      }

      auto header = header_map();

      struct stat stbuf;
      if (stat(path.c_str(), &stbuf) == 0) {
        header.emplace("content-length",
                       header_value{std::to_string(stbuf.st_size)});
        header.emplace("last-modified",
                       header_value{http_date(stbuf.st_mtime)});
      }
      res.write_head(200, std::move(header));
      res.end(file_generator_from_fd(fd));
  });

  if (server.listen_and_serve(ec, tls, "localhost", "3000")) {
    std::cerr << "error: " << ec.message() << std::endl;
  }
}