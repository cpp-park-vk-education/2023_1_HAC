#include "logger.h"

FileLogger::FileLogger(const std::string& filename) {
    file.open(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open the log file: " << filename << std::endl;
    }
}

FileLogger::~FileLogger() {
    if (file.is_open()) {
        file.close();
    }
}

FileLogger& FileLogger::getInstance(const std::string& filename) {
    static FileLogger instance(filename);
    return instance;
}

void FileLogger::log(const std::string& message) {
    if (!file.is_open()) {
        std::cerr << "Log file is not open." << std::endl;
        return;
    }

    std::time_t now = std::time(nullptr);
    std::tm* local_time_UTC = std::localtime(&now);
    std::time_t local_now = std::mktime(local_time_UTC);
    local_now += 3 * 3600; // Применяем смещение в секундах
    std::tm* local_time = std::localtime(&local_now);

    std::stringstream log_steam;
    std::cout << "[" << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << "] " << message << std::endl;

    file << log_steam.str();
    file.flush();
}
