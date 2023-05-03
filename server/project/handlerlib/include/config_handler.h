#pragma once // NO_LINT

#include <string>
#include <chrono>


struct DBRequestProtocol;
struct DBResponseProtocol;

namespace handlers {

using time_ =  std::chrono::time_point<std::chrono::system_clock>;

struct ProtocolAPI {
    std::string name_stock_hub;
    std::string name_packet_stock;
    time_ start_time;
    time_ end_time;
    std::string token;
};



} // namespace handlers 
