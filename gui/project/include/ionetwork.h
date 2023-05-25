#ifndef TEST_VK_PROJECT_IONETWORK_H
#define TEST_VK_PROJECT_IONETWORK_H

#include <QtNetwork/QNetworkAccessManager>
#include <QUrlQuery>

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <QNetworkReply>
#include <QUrlQuery>

#include "../include/ionetwork_interface.h"

class IONetwork : public IONetworkInterface, QObject {
public:
    void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(const Error& error_state)>
                                     callback) override;

    void onFirstFinishedPost(QNetworkReply* reply, std::string user,
                             std::function<void(const Error& error_state)> callback);

    void onFinishedPost(QNetworkReply* reply, std::string user,
                     std::function<void(const Error& error_state)> callback);
    void onFinishedPostExit(QNetworkReply* reply, std::string user,
                     std::function<void(const Error& error_state)> callback);

    void GetRequest(const std::string& url, std::istream& body,
                            std::function<void(std::istream& network_output,
                                    const Error& error_state)> callback)
                                    override;

    void onFinishedGet(QNetworkReply* reply,
                       std::function<void(std::istream& network_output,
                               const Error& error_state)> callback);

    void setConfig(const std::string& action_name) override;
    void setCookie(const std::string& cookie_data) override;
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override;

    std::string parseGetData(const std::string answer);

private:
    QNetworkAccessManager network_manager;
    ptr_to_iwindow_manager window_manager_ptr;
    std::string action_name_;
    std::string cookie;
};

#endif //TEST_VK_PROJECT_IONETWORK_H
