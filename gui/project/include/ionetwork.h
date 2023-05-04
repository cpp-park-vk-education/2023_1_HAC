#ifndef TEST_VK_PROJECT_IONETWORK_H
#define TEST_VK_PROJECT_IONETWORK_H

#include <QtNetwork/QNetworkAccessManager>

#include <string>
//#include <functional>

#include "../include/ionetwork_interface.h"


class IONetwork : public IONetworkInterface, QObject {
public:
    virtual void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(const Error& error_state)>
                                     callback) override {};
    virtual void GetRequest(const std::string& url, const MainData& body,
                            std::function<void(const std::istream& network_output,
                                    const Error& error_state)> callback)
                                    override {};
    virtual void setConfig(const std::string& host) override {};//????
private:
    QNetworkAccessManager* network_manager;
};

#endif //TEST_VK_PROJECT_IONETWORK_H
