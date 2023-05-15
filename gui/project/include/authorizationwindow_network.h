#ifndef AUTHORIZATIONWINDOW_NETWORK_H
#define AUTHORIZATIONWINDOW_NETWORK_H
#include <iostream>
#include <string>
#include <memory>
//#include "../include/usecase_authorizationwindow_interface.h"
#include "../include/authorizationwindow_network_interface.h"

class NetworkAuthorizationWindow : public INetworkAuthorizationWindow {
public:
    void setAuthorizationHandler(ptr_to_authorization_handler auth_handler_ptr)
    override {
        authorization_handler_ptr = auth_handler_ptr;
    }
    void setAuthorizationNetwork(ptr_to_inetwork net_ptr) override {
        network_ptr = net_ptr;
    }
    void getAuthorization(const AuthInput& auth_params) override {
        std::cout <<"here I am in AN"<<std::endl;
        auth_params_ = auth_params;
        //std::string auth_line = "login\t" + auth_params.login + "\t" + "password\t" + auth_params.password;
        //network_ptr->PostRequest(url_, );
        std::string auth_line = auth_params.login + "\t" + auth_params.password;
        network_ptr->PostRequest(url_, auth_line,
                                 [this](const Error& error_state)
                                 {onGetAuthorizationResponse
                                         (error_state);});
    }
    void onGetAuthorizationResponse(const Error& error_state) override {
        std::cout << "In Auth Response" <<std::endl;
        std::cout << error_state.type << ' ' <<error_state.message << std::endl;
        if (error_state.type == "0") {
             authorization_handler_ptr->passToMain(error_state.message);
        } else {
            authorization_handler_ptr->sendError(error_state);
        }
    }
    void setUrl(const std::string& url) override {
        url_ = url;
        //std::cout<<"!" <<url_<<"!"<<std::endl;
    }
private:
    ptr_to_inetwork network_ptr;
    ptr_to_authorization_handler authorization_handler_ptr;
    AuthInput auth_params_;
    std::string url_;
};

#endif // AUTHORIZATIONWINDOW_NETWORK_H
