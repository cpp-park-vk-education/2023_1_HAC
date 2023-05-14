#include "listener.h"

Listener::Listener(net::io_context& ioc, 
                   tcp::endpoint endpoints, IRouterAdapter* router) 
                   : ioc_(ioc),
                     acceptor_(net::make_strand(ioc)) {};

void Listener::Run(){};
void Listener::doAccept(){};
void Listener::onAccept(){};