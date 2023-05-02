#pragma once
#include <iostream>
#include <memory>
#include <pqxx/pqxx>
//#include "idatabase.hpp"
#include "databaseerrors.hpp"

class IDataBase{
public:
    virtual bool IsOpen() = 0;
    virtual bool SendQuery(const std::string& query) = 0;
    virtual std::vector<std::vector<std::string>> GetData(const std::string& query) = 0;
    virtual std::vector<std::string> GetRow(const std::string& query) = 0;
};


class PostgresServer: public IDataBase {
public:
    PostgresServer();
    PostgresServer(std::string addr, std::string port, std::string db_name, std::string user, std::string pass);

    bool IsOpen() override;
    std::vector<std::vector<std::string>> GetData(const std::string& query) override;
    std::vector<std::string> GetRow(const std::string& query) override;
    bool SendQuery(const std::string& query) override;

private:
    std::string host_addr_;
    std::string port_;
    std::string db_name_;
    std::string user_;
    std::string password_;
    std::shared_ptr<pqxx::connection> conn_;
    void Connect();
};