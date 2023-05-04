#include "postgresserver.hpp"

using namespace database;

PostgresServer::PostgresServer(): host_addr_("127.0.0.1"), port_("5432"), db_name_("marketmentor"),
     user_("marketmentor_server"), password_("marketmentor_password"){
        Connect();
}

PostgresServer::PostgresServer(const std::string&  addr, const std::string&  port,
         const std::string&  db_name, const std::string&  user, const std::string&  pass): host_addr_(addr),
    port_(port), db_name_(db_name), user_(user), password_(pass){
    Connect();
}

bool PostgresServer::IsOpen() {
    return true;
}

Json::Value PostgresServer::GetData(const std::string& query) {
    std::string json_string = "{\"test\": 1}";
    Json::Value json;
    Json::Reader reader;
    reader.parse(json_string, json);
    return json;
}

Json::Value PostgresServer::GetRow(const std::string& query) {
    std::string json_string = "{\"test\": 1}";
    Json::Value json;
    Json::Reader reader;
    reader.parse(json_string, json);
    return json;
}

bool PostgresServer::SendQuery(const std::string& query) {
    return true;
}

void PostgresServer::Connect() {
    return;
}

