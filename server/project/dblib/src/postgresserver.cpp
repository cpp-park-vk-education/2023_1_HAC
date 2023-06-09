#include <iostream>
#include "postgresserver.hpp"

database::PostgresServer::PostgresServer(): host_addr_("0.0.0.0"), port_("5433"), db_name_("marketmentor"),
     user_("marketmentor_server"), password_("marketmentor_password") {
}

database::PostgresServer::PostgresServer(const std::string&  addr, const std::string&  port,
         const std::string&  db_name, const std::string&  user, const std::string&  pass): host_addr_(addr),
        port_(port), db_name_(db_name), user_(user), password_(pass) {
}


bool database::PostgresServer::IsOpen() {
    if (conn_ == nullptr) {
        return false;
    }

    return conn_->is_open();
}


bool database::PostgresServer::Connect() {
    std::string connecting_string = "dbname = " + db_name_+ " user = " + user_ + " password = " + password_ +
        " hostaddr = " + host_addr_ + " port = " + port_;
    std::cout << connecting_string << std::endl;
    try {
        conn_ = std::make_shared<pqxx::connection>(connecting_string);
    } 
    catch (pqxx::failure const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}


bool database::PostgresServer::SendQuery(const std::string& query) {
pqxx::work transaction{*conn_};
    try{
        transaction.exec(query);
        transaction.commit();
    }
    catch (pqxx::transaction_rollback const &e) {
        std::cerr << e.what() << std::endl;
        return false;

    }
    catch (pqxx::sql_error const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }   
    catch (pqxx::failure const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;   
}

Json::Value database::PostgresServer::GetRow(const std::string& query) {
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
    catch (pqxx::unexpected_rows const &e) {
        std::cerr << e.what() << std::endl;
        throw ElementNotExist("Bad query: " + query);
    }

    Json::Value result;
    int field_number = 0;
    for (auto field = row.begin(); field != row.end(); field++){
        if (!field.is_null()){
            result[field_number++] = field.as<std::string>();
        }
        else{
            result[field_number++] = "NULL";
        }
    }

    return result;
}

Json::Value database::PostgresServer::GetData(const std::string& query) {
    pqxx::nontransaction tx{*conn_};
    pqxx::result table;
    
    try {
        table = tx.exec(query);
    }
    catch (pqxx::plpgsql_no_data_found const &e) {
        std::cerr << e.what() << std::endl;
        throw ElementNotExist("Element doesn't exist in database");
    }
    catch (pqxx::unexpected_rows const &e) {
        std::cerr << e.what() << std::endl;
        throw ElementNotExist("Bad query: " + query);
    }   
    catch (pqxx::sql_error const &e) {
        std::cerr << e.what() << std::endl;
        throw SqlError("Bad query: " + query);
    }   
    catch (pqxx::failure const &e) {
        std::cerr << e.what() << std::endl;
        throw ConnectError("Broken connection to database");
    }   

    Json::Value result;
    Json::Value row_buffer;
    int row_number = 0;
    int field_number = 0;

    for (auto row = table.begin(); row != table.end(); row++){
        for (auto field = row.begin(); field != row.end(); field++){
            if (!field.is_null()) {
                row_buffer[field_number++] = field.as<std::string>();

            }
            else {
                row_buffer[field_number++] = "NULL";
            }
        }
        result[row_number++] = row_buffer;
        field_number = 0;
    }
    
    return result;
}

