#ifndef AUTHORIZATIONWINDOW_NETWORK_H
#define AUTHORIZATIONWINDOW_NETWORK_H
#include <iostream>
#include <string>
#include <memory>

#include "../include/authorizationwindow_network_interface.h"

class NetworkAuthorizationWindow : public INetworkAuthorizationWindow {
public:
    void setAuthorizationHandler(ptr_to_authorization_handler auth_handler_ptr)
    override;
    void setAuthorizationNetwork(ptr_to_inetwork net_ptr) override;
    void getAuthorization(const AuthInput& auth_params) override;
    void onGetAuthorizationResponse(const Error& error_state) override;
    void setCookie(const std::string& user, const std::string& cookie_line) override;
    void onSetCookieResponse(const Error& error_state) override;
    void setUrl(const std::string& url) override;
private:
    ptr_to_inetwork network_ptr;
    ptr_to_authorization_handler authorization_handler_ptr;
    AuthInput auth_params_;
    std::string url_;
};

#endif // AUTHORIZATIONWINDOW_NETWORK_H
