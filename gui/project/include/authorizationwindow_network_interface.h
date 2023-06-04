#ifndef AUTHORIZATIONWINDOW_NETWORK_INTERFACE_H
#define AUTHORIZATIONWINDOW_NETWORK_INTERFACE_H

#include <memory>

#include "../include/ionetwork_interface.h"
#include "../include/usecase_authorizationwindow_interface.h"

struct AuthInput {
    std::string login;
    std::string password;
};

class IONetworkInterface;
using ptr_to_inetwork = std::shared_ptr<IONetworkInterface>;

class IHandlerAuthorizationWindow;
using ptr_to_authorization_handler = std::shared_ptr<IHandlerAuthorizationWindow>;

class INetworkAuthorizationWindow {
public:
    virtual void setAuthorizationHandler(
            ptr_to_authorization_handler auth_handler_ptr) = 0;
    virtual void setAuthorizationNetwork(ptr_to_inetwork net_ptr) = 0;
    virtual void getAuthorization(const AuthInput& auth_params) = 0;
    virtual  void onGetAuthorizationResponse(const Error& error_state)= 0;
    virtual void setCookie(const std::string& user, const std::string& cookie_line) = 0;
    virtual void onSetCookieResponse(const Error& error_state) = 0;
    virtual void setUrl(const std::string& url) = 0;
};

#endif // AUTHORIZATIONWINDOW_iNTERFACE_NETWORK_H
