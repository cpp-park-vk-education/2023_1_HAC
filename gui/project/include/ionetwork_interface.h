#ifndef TEST_VK_PROJECT_IONETWORK_INTERFACE_H
#define TEST_VK_PROJECT_IONETWORK_INTERFACE_H

#include <functional>
#include <string>
#include <vector>
#include <memory>

#include "../include/guicontroller_interface.h"

using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

struct Error {
    std::string type;
    std::string message;
};

struct MainData {
    std::string operation_title;
    std::string stock_name;
    int lag;
    int window_size;
};

class IONetworkInterface {
public:
    virtual void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(
                                     const Error& error_state)> callback) = 0;
    virtual void GetRequest(const std::string& url, std::istream& body,
                            std::function<void(
                                    std::istream& network_output,
                                    const Error& error_state)> callback) = 0;
    virtual void setConfig(const std::string& host) = 0;
    virtual void setCookie(const std::string& cookie_data) = 0;

    virtual void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) = 0;
};

#endif //TEST_VK_PROJECT_IONETWORK_INTERFACE_H
