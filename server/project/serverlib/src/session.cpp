#include "session.h"

Session::Session(tcp::socket&& socket): stream_(std::move(socket)){};

void Session::setRouterAdapter(RouterAdapter* router_adapter) {}
void Session::run(){};

void Session::doRead(){};
void Session::onRead(){};
void Session::sendResponce(){};
void Session::doClouse(){};
