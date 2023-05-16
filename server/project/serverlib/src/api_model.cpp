#include "api_model.h"

api::APIModelRequest::APIModelRequest(){};
Json::Value api::APIModelRequest::getData(const controllers::TimeSeriesPredicts& samples_data){
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string("25.21.238.202"), 9998);

    socket.connect(endpoint);

    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET / HTTP/1.1\r\n";
    request_stream << "Host: 127.0.0.1\r\n";
    request_stream << "Connection: close\r\n\r\n";
    request_stream << "asljd: close\r\n\r\n";
    request_stream << "Window_size: " + std::to_string(samples_data.window_size) + "\r\n\r\n";
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


    return 0;

};

void api::APIModelRequest::doConnect(std::string path){};
IHTTPResponse* api::APIModelRequest::onConnect(ssl::stream<tcp::socket> stream){};