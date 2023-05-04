#include "dbcontroller.hpp"

using namespace dbcontroller;
using namespace repository;
using namespace dbcontroller;


DataBaseController::DataBaseController(): database_(new PostgresServer()), clien_rep_(new ClientRepository(database_)),
    timeseries_rep_(new TimeSeriesRepository(database_)), subscription_rep_(new SubscriptionRepository(database_)) {
}

DataBaseController::DataBaseController(const std::shared_ptr<IClientRepository>& client_rep, 
        const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, const std::shared_ptr<ISubscriptionRepository>& subscription_rep): 
        clien_rep_(client_rep), timeseries_rep_(timeseries_rep), subscription_rep_(subscription_rep)
{
}

Json::Value DataRequest(const Json::Value& request) {
    std::string json_string = "{\"test\": 1}";
    Json::Value json;
    Json::Reader reader;
    reader.parse(json_string, json);
    return json;
}


bool DataBaseController::TimeSeriesPost(const std::shared_ptr<TimeSeriesData>& data) {
    return true;
}
std::shared_ptr<TimeSeriesData> DataBaseController::TimeSeriesGet(const std::string& name_stock, const std::string& len_lags) {
    return nullptr;
}

bool DataBaseController::ClientRequestPost(const std::shared_ptr<ClientData>& data) {
    return true;
}

std::shared_ptr<ClientData> DataBaseController::ClientRequestGet(const std::string& key) {
    return nullptr;
}

bool DataBaseController::ClientRequestUpdate(const std::shared_ptr<ClientData> data) {
    return true;
}