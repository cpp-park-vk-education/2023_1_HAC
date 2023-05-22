#include "update_data_job.h"
#include <ctime>
#include <iomanip>

void printHelloWorldByTimer() {
    while (true) {
        std::time_t now = std::time(nullptr);
        std::tm* local_time_UTC = std::localtime(&now);
        std::time_t local_now = std::mktime(local_time_UTC);
        local_now += 3 * 3600; // Применяем смещение в секундах
        std::tm* local_time = std::localtime(&local_now);
      //  std::cout << "------------------------------" << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}