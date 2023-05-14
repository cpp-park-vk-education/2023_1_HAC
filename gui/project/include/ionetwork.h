#ifndef TEST_VK_PROJECT_IONETWORK_H
#define TEST_VK_PROJECT_IONETWORK_H

#include <QtNetwork/QNetworkAccessManager>

#include <string>
//#include <functional>

#include "../include/ionetwork_interface.h"


class IONetwork : public IONetworkInterface, QObject {
public:
    void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(const Error& error_state)>
                                     callback) override {}
    void GetRequest(const std::string& url, const MainData& body,
                            std::function<void(const std::istream& network_output,
                                    const Error& error_state)> callback)
                                    override {}
    void setConfig(const std::string& host) override {}
private:
    QNetworkAccessManager* network_manager;
    std::string host_;
};

#endif //TEST_VK_PROJECT_IONETWORK_H
