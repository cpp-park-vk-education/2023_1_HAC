#pragma once // NO_LINT

#include <string>
#include <chrono>

namespace handlers {

struct ProtocolAPI {
    std::string name_stock_hub;
    std::string name_packet_stock;
    std::string start_time;
    std::string end_time;
    std::string token;
};



} // namespace handlers 
