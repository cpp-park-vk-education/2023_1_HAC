#pragma once // NO_LINT
#include <memory>
#include <pqxx/pqxx>
#include "idatabase.hpp"

namespace database {

    class PostgresServer: public IDataBase {
    public:
        PostgresServer();
        PostgresServer(const std::string& addr, const std::string& port,
            const std::string&  db_name, const std::string&  user, const std::string&  pass);

        bool IsOpen() override;
        Json::Value GetData(const std::string& query) override;
        Json::Value GetRow(const std::string& query) override;
        bool SendQuery(const std::string& query) override;
        bool Connect() override;

    private:
        std::string host_addr_;
        std::string port_;
        std::string db_name_;
        std::string user_;
        std::string password_;
        std::shared_ptr<pqxx::connection> conn_;
    };

}
