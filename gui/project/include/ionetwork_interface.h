#ifndef TEST_VK_PROJECT_IONETWORK_INTERFACE_H
#define TEST_VK_PROJECT_IONETWORK_INTERFACE_H

#include <functional>

struct Error {
    std::string type;
    std::string message;
};

struct MainData {
    std::string operation_title;
    std::string stock_name;
};

class IONetworkInterface {
public:
    virtual void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(
                                     const Error& error_state)> callback) = 0;
    virtual void GetRequest(const std::string& url, const MainData& body,
                            std::function<void(
                                    const std::istream& network_output,
                                    const Error& error_state)> callback) = 0;
    virtual void setConfig(const std::string& host) = 0;
};

#endif //TEST_VK_PROJECT_IONETWORK_INTERFACE_H
