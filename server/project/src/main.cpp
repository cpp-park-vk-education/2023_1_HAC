#include "server.h"
#include "logger.h"
#include <iostream>

int main(int argc, char* argv[])
{   
    FileLogger& logger = FileLogger::getInstance();

    std::string str = "../../project/utils/config_server.txt";
    try {
        Server serv(str);
    }
    catch(const market_mentor::MarketMentorException& e){
        logger.log(e.what());
    }

    return 0;
}
