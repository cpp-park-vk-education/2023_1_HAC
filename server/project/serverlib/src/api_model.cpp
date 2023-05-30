#include "api_model.h"
#include <iostream>

api::APIModelRequest::APIModelRequest(){};
Json::Value api::APIModelRequest::getData(const controllers::TimeSeriesPredicts samples_data){ 
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string("62.84.127.93"), 9950);

    socket.connect(endpoint);

    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET / HTTP/1.1\r\n";
    request_stream << "lenpredict: " + std::to_string(samples_data.lenpredict) + "\r\n";
    request_stream << "stock_name: " + samples_data.stock_name + "\r\n";
    request_stream << "action: " + samples_data.action + "\r\n";
    std::string samples_data_body; 
    for (auto i : samples_data.matrix_samples){
        samples_data_body += std::to_string(i);
        samples_data_body += " ";
    }
    request_stream << "data: " + samples_data_body + "\r\n\r\n";    
    boost::asio::write(socket, request);

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
    std::string response_string(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));

    //std::cerr << response_string;    
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
    if (samples_data.action == "predict") {
        std::string data_string = headers["data"];

        std::vector<double> parsed_data;
        std::string now_value;
        for (size_t i = 1; i < data_string.length(); ++i){
        
            if (data_string[i] == ' ' && now_value.length()) {
               // std::cerr << now_value << std::endl;
                parsed_data.push_back(std::stod(now_value));
                now_value = "";
            } else if (data_string[i] != ' ' ) {
                now_value += data_string[i]; 
            }
            
        }
      // parsed_data.push_back(std::stod(now_value));

        json_resp["status"] = true;
        Json::Value json_param;
        std::string number;
        for (int i = 0; i < parsed_data.size(); i++) {  
            json_param[i] = parsed_data[i];
        };
        json_resp["param"] = json_param;
       // std::cerr << json_resp.toStyledString() << std::endl;
    }
   // std::cerr << "$$$$$$$$$$$$";
    return json_resp;
};

void api::APIModelRequest::doConnect(std::string path){};
IHTTPResponse* api::APIModelRequest::onConnect(ssl::stream<tcp::socket> stream){};
