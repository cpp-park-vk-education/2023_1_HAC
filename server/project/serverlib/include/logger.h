#pragma once // NO_LINT

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>

class FileLogger {
private:
    std::ofstream file;

    FileLogger(const std::string& filename);

public:
    ~FileLogger();

    static FileLogger& getInstance(const std::string& filename = "server.log");

    void log(const std::string& message);
};
