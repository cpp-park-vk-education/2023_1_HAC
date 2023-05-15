#include "listener.h"
#include <iostream>


Listener::Listener(net::io_context& ioc, 
                   tcp::endpoint endpoint) 
                   : ioc_(ioc),
                     acceptor_(net::make_strand(ioc)) {
  beast::error_code ec;

  // Open the acceptor
  acceptor_.open(endpoint.protocol(), ec);
  // Allow address reuse
  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    // Bind to the server address
  acceptor_.bind(endpoint, ec);

    // Start listening for connections
  acceptor_.listen(
    net::socket_base::max_listen_connections, ec);
};

 void Listener::Run(){
   doAccept();
 };
 void Listener::doAccept(){
   // The new connection gets its own strand
   std::cerr << "Listener DO Accept" << std::endl;
   acceptor_.async_accept( 
       net::make_strand(ioc_),
       beast::bind_front_handler(
       &Listener::onAccept,
       shared_from_this()));
 };
void Listener::onAccept(beast::error_code ec, tcp::socket socket){
  std::cerr << "Listener On Accept";
  std::make_shared<Session>(
                std::move(socket))->run();
  Listener::doAccept();
};