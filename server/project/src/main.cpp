#include "server.h"
#include "logger.h"
#include <iostream>

int main(int argc, char* argv[])
{   
    FileLogger& logger = FileLogger::getInstance();

    std::string server_config = "../../project/utils/config_server.txt";
    try {
        Server serv(server_config);
    }
    catch(const market_mentor::MarketMentorException& e){
        logger.log(e.what());
    }
    catch(std::exception &e){
        logger.log(std::string("Catched unknown error: ") + e.what());
    }
    return 0;
}
