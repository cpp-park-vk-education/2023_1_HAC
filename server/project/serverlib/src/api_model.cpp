#include "api_model.h"
#include "logger.h"
#include <fstream>

const std::string HTTP_REQUEST_TYPE_HEADER = "GET / HTTP/1.1";
const std::string LENPREDICT_HEADER = "lenpredict:";
const std::string STOCK_NAME_HEADER = "stock_name: ";
const std::string ACTION_HEADER = "action: ";
const std::string DATA_HEADER = "data: "; 
const std::string DELIMITER_DATA = " ";
const std::string PREDICT_HEADER = "predict";
const std::string RESPONSE_DATA_HEADER = "data";
const char RESPONSE_DELIMMETER_DATA = ' ';
const std::string JSON_RESPONSE_STATUS_HEADER = "status";
const std::string JSON_RESPONSE_DATA_HEADER = "param";

api::APIModelRequest::APIModelRequest(const std::string& path_to_config_file){
    std::ifstream config_stream (path_to_config_file);
    if (!config_stream) {
        throw market_mentor::InvalidServerConfig("Wrong model config path");
    }
    std::string host, port;
    config_stream >> host >> port;
    if (!host.length() || !port.length()) {
        throw market_mentor::InvalidServerConfig("Invalid model config data");
    }  
    try {
        host_ = host;
        port_ = stoi(port);
    } 
    catch(std::invalid_argument){
        throw market_mentor::InvalidServerConfig("Invalid model config data");
    }
};

Json::Value api::APIModelRequest::getData(const controllers::TimeSeriesPredicts samples_data){ 
    FileLogger& logger = FileLogger::getInstance();
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);

    try {
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string(host_), port_);

        socket.connect(endpoint);
    }
    catch (...) {
        throw market_mentor::ConnectionException("Cannot connect to model");
    }

    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << HTTP_REQUEST_TYPE_HEADER + "\r\n";
    request_stream << LENPREDICT_HEADER + std::to_string(samples_data.lenpredict) + "\r\n";
    request_stream << STOCK_NAME_HEADER + samples_data.stock_name + "\r\n";
    request_stream << ACTION_HEADER + samples_data.action + "\r\n";
    std::string samples_data_body; 
    for (auto i : samples_data.matrix_samples){
        samples_data_body += std::to_string(i);
        samples_data_body += DELIMITER_DATA;
    }
    request_stream << DATA_HEADER + samples_data_body + "\r\n\r\n";    
    boost::asio::write(socket, request);

    std::string response_string;
    try{
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        boost::asio::streambuf::const_buffers_type bufs = response.data();
        response_string = std::string(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));
    } catch(...){
        throw market_mentor::ReedDataException("Error reading data from model");
    }

    std::map<std::string, std::string> headers;

    std::istringstream iss(response_string);
    std::string line;
    while (std::getline(iss, line) && line != "\r") {
        std::size_t colonPos = line.find(":");
        if (colonPos != std::string::npos) {
            std::string headerName = line.substr(0, colonPos);
            std::string headerValue = line.substr(colonPos + 2);  // +2 для пропуска ": " после имени заголовка
            headers[headerName] = headerValue;
        }
    }

    Json::Value json_resp;
    if (samples_data.action == PREDICT_HEADER) {
        try {
            json_resp = parseData(headers);
        } catch(...) {
            logger.log("Invalid response from model");
            json_resp[JSON_RESPONSE_STATUS_HEADER] = false;
        }  
    }

    return json_resp;
};

Json::Value api::APIModelRequest::parseData(const std::map<std::string, std::string>& headers) {
    Json::Value json_resp;
    std::string data_string = headers.at(RESPONSE_DATA_HEADER);

    std::vector<double> parsed_data;
    std::string now_value;
    for (size_t i = 1; i < data_string.length(); ++i){
        
        if (data_string[i] == RESPONSE_DELIMMETER_DATA && now_value.length()) {
            parsed_data.push_back(std::stod(now_value));
            now_value = "";
        } else if (data_string[i] != RESPONSE_DELIMMETER_DATA) {
            now_value += data_string[i]; 
        }
            
    }
    if (now_value.length()){
        parsed_data.push_back(std::stod(now_value));
    }
    parsed_data.push_back(std::stod(now_value));

    json_resp[JSON_RESPONSE_STATUS_HEADER] = true;
    Json::Value json_param;
    std::string number;
    for (int i = 0; i < parsed_data.size(); i++) {  
        json_param[i] = parsed_data[i];
    };

    json_resp[JSON_RESPONSE_DATA_HEADER] = json_param;
    return json_resp;
}
    