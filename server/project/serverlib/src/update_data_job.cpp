#include "update_data_job.h"
#include <ctime>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <thread>


void getNewDataByTimer(controllers::IUpdateDataController* update_controller){
    std::time_t now = std::time(nullptr);
    std::tm* local_time_UTC = std::localtime(&now);
    std::time_t local_now = std::mktime(local_time_UTC);
    local_now += 3 * 3600;
    std::tm* local_time = std::localtime(&local_now);
    int time_to_sleep_first = 61 - local_time->tm_min; // 61 минута, чтобы ходить на биржу в 01 минуту слудующего часа 
    //std::this_thread::sleep_for(std::chrono::minutes(time_to_sleep_first));

    // сделать с конфигом
    handlers::ProtocolAPI protocol;
    protocol.name_stock_hub = "finnhub.io";
    protocol.name_packet_stock = "AAPL";
    protocol.token = "chmd799r01qjapkrdjngchmd799r01qjapkrdjo0";
    while (true) {
        std::time_t current_time = std::time(nullptr);
        std::time_t two_minutes_ago = current_time - 2 * 60;

        protocol.start_time = std::to_string(two_minutes_ago);
        protocol.end_time = std::to_string(current_time);
        //update_controller->updateData(protocol);
        std::this_thread::sleep_for(std::chrono::minutes(60));
    }
}