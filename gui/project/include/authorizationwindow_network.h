#ifndef AUTHORIZATIONWINDOW_NETWORK_H
#define AUTHORIZATIONWINDOW_NETWORK_H

#include <string>
#include <memory>
//#include "../include/usecase_authorizationwindow_interface.h"
#include "../include/authorizationwindow_network_interface.h"

class NetworkAuthorizationWindow : public INetworkAuthorizationWindow {
public:
    void setAuthorizationHandler(ptr_to_authorization_handler auth_handler_ptr) override {};
    void setAuthorizationNetwork(ptr_to_inetwork net_ptr) override {};
    void getAuthorization(const AuthInput& auth_params) override {};
    void onGetAuthorizationResponse(const Error& error_state) override {};
private:
    ptr_to_inetwork network_ptr;
    ptr_to_authorization_handler authorization_handler_ptr;
    AuthInput auth_params_;
};

#endif // AUTHORIZATIONWINDOW_NETWORK_H
