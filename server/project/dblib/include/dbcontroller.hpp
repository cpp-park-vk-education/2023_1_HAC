// #pragma once
// #include <iostream>
// #include <memory>
// #include "postgresserver.hpp"
// #include "repositories.hpp"
// #include "data.hpp"


// // using time_ =  td::chrono::time_point<std::chrono::system_clock>;
// // using DataRequestPtr = std::unique_ptr<DataRequest>;
// // using DataResponsePtr = std::unique_ptr<DataResponse>;

// enum TypeRequest {
//     GET_REQUEST,
//     POST_REQUEST, 
//     UPDATE_REQUEST
// };

// enum TypeData {
//     CLIENT_DATA,
//     TIMESERIES_DATA
// };

// // Запрос от сервера
// template <typename Data>
// struct DBRequestProtocol {
//     TypeRequest type_request;
// //    TypeData type_data;
//     std::unique_ptr<Data> data;
// };

// // Ответ от БД
// template <typename Data>
// struct DBResponseProtocol {
//     bool status;
//     std::unique_ptr<Data> data;
// };


// class IDataBaseController {
// public:
//     virtual DBResponseProtocol<ClientData> DataRequest(const DBRequestProtocol<ClientData>& request) = 0;
//     virtual DBResponseProtocol<TimeSeriesData> DataRequest(const DBRequestProtocol<TimeSeriesData>& request) = 0;
//     virtual DBResponseProtocol<AuthorizeData> DataRequest(const DBRequestProtocol<AuthorizeData>& request) = 0;
//     virtual DBResponseProtocol<TimeSeriesData> DataRequest(const DBRequestProtocol<TimeSeriesRequest>& request) = 0;
// protected:
//     // TimeSeries
//     virtual bool TimeSeriesPost(const std::unique_ptr<IDataRequest>& data) = 0;
//     virtual std::shared_ptr<IData> TimeSeriesGet(const std::string& name_stock, const std::string& len_lags) = 0;
//     // Client
//     virtual bool ClientRequestPost(const std::unique_ptr<ClientData>& data) = 0;
//     virtual std::shared_ptr<IData> ClientRequestGet(const std::string& key) = 0;
//     virtual bool ClientRequestUpdate(const std::unique_ptr<SubscriptionData> data) = 0;
// };


// class DataBaseController: public IDataBaseController {
// public:
//     DataBaseController();
//     DBResponseProtocol<ClientData> DataRequest(const DBRequestProtocol<ClientData>& request);
//     DBResponseProtocol<TimeSeriesData> DataRequest(const DBRequestProtocol<TimeSeriesData>& request);
//     DBResponseProtocol<TimeSeriesData> DataRequest(const DBRequestProtocol<TimeSeriesRequest>& request);
//     DBResponseProtocol<AuthorizeData> DataRequest(const DBRequestProtocol<AuthorizeData>& request);

// private:
//     virtual bool TimeSeriesPost(const std::unique_ptr<IDataRequest>& data) = 0;
//     virtual std::shared_ptr<IData> TimeSeriesGet(const std::string& name_stock, const std::string& len_lags) = 0;
//     virtual bool ClientRequestPost(const std::unique_ptr<IData>& data) = 0;
//     virtual std::shared_ptr<IData> ClientRequestGet(const std::string& key) = 0;
//     virtual bool ClientRequestUpdate(const std::unique_ptr<IData> data) = 0;

//     static std::string host_addr_;
//     static std::string port_;
//     static std::string db_name_;
//     static std::string user_;
//     static std::string password_;
//     std::shared_ptr<IDataBase> database_;
//     std::shared_ptr<ISubscriptionRepository> subscription_rep_;
//     std::shared_ptr<ITimeSeriesRepository> timeseries_rep_;
//     std::shared_ptr<IClientRepository> clien_rep_;
// }