#include "postgresserver.hpp"

PostgresServer::PostgresServer(): host_addr_("127.0.0.1"), port_("5432"), db_name_("marketmentor"),
     user_("marketmentor_server"), password_("marketmentor_password"){
        Connect();
}

PostgresServer::PostgresServer(std::string addr, std::string port, std::string db_name, std::string user, std::string pass): host_addr_(addr),
    port_(port), db_name_(db_name), user_(user), password_(pass){
    Connect();
}

bool PostgresServer::IsOpen() {
    return conn_->is_open();
}

std::vector<std::vector<std::string>> PostgresServer::GetData(const std::string& query) {
    pqxx::nontransaction tx{*conn_};
    pqxx::result table;
    
    try {
        table = tx.exec(query);
    }
    catch (pqxx::sql_error const &e) {
        std::cerr << e.what() << std::endl;
        throw SqlError("Bad query: " + query);
    }   
    catch (pqxx::failure const &e) {
        std::cerr << e.what() << std::endl;
        throw ConnectError("Broken connection to database");
    }   

    std::vector<std::vector<std::string>> result;
    std::vector<std::string> row_buffer;
    for (auto row = table.begin(); row != table.end(); row++){
        for (auto field = row.begin(); field != row.end(); field++){
            if (!field.is_null()){
                row_buffer.push_back(field.as<std::string>());
            }
            else{
                row_buffer.push_back("NULL");
            }
        }
        result.push_back(row_buffer);
        row_buffer.clear();
    }
    
    return result;
}

std::vector<std::string> PostgresServer::GetRow(const std::string& query) {
    pqxx::nontransaction tx{*conn_};
    pqxx::row row;
    try {
        row = tx.exec1(query);
    }
    catch (pqxx::sql_error const &e) {
        std::cerr << e.what() << std::endl;
        throw SqlError("Bad query: " + query);
    }   
    catch (pqxx::failure const &e) {
        std::cerr << e.what() << std::endl;
        throw ConnectError("Broken connection to database");
    }   

    std::vector<std::string> buffer;    
    for (auto field = row.begin(); field != row.end(); field++){
        if (!field.is_null()){
            buffer.push_back(field.as<std::string>());
        }
        else{
            buffer.push_back("NULL");
        }
    }
    return buffer;
}

bool PostgresServer::SendQuery(const std::string& query) {
    pqxx::work transaction{*conn_};
    try{
        transaction.exec(query);
        transaction.commit();
    }
    catch (pqxx::transaction_rollback const &e) {
        std::cerr << e.what() << std::endl;
    //    throw TransactionError("Failed transaction: " + query);
        return false;

    }
    catch (pqxx::sql_error const &e) {
        std::cerr << e.what() << std::endl;
   //     throw SqlError("Bad query: " + query);
        return false;
    }   
    catch (pqxx::failure const &e) {
        std::cerr << e.what() << std::endl;
    //    throw ConnectError("Broken connection to database");
        return false;
    }
    return true;   
}

void PostgresServer::Connect() {
    std::string connecting_string = "dbname = " + db_name_+ " user = " + user_ + " password = " + password_ +
        " hostaddr = " + host_addr_ + " port = " + port_;
    try {
        conn_ = std::make_shared<pqxx::connection>(connecting_string);
    } catch (pqxx::failure const &e) {
        std::cerr << e.what() << std::endl;
        throw ConnectError("Failed connect to database " + db_name_);
    }
}

