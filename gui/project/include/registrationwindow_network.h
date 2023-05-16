#ifndef REGISTRATIONWINDOW_NETWORK_H
#define REGISTRATIONWINDOW_NETWORK_H

//#include "project/include/ionetwork_interface.h"
//#include "../include/usecase_registrationwindow_interface.h"
#include "../include/registrationwindow_network_interface.h"
#include <string>

class NetworkRegistrationWindow : public INetworkRegistrationWindow {
public:
    void setRegistrationHandler(ptr_to_registration_handler reg_handler_ptr) override {
        registration_handler_ptr = reg_handler_ptr;
    };
    void setRegistrationNetwork(ptr_to_inetwork net_ptr) override {
        network_ptr = net_ptr;
    };
    void getRegistration(const RegInput& reg_params) override {
        std::cout << "in get reg" <<std::endl;
        reg_params_ = reg_params;
        //std::string auth_line = "login\t" + auth_params.login + "\t" + "password\t" + auth_params.password;
        //network_ptr->PostRequest(url_, );
        std::string auth_line = reg_params.login + "\t" + reg_params.password
                + "\t" + reg_params.email;
        std::cout<<"*" <<auth_line <<"*"<<std::endl;

        network_ptr->setConfig("REGISTRATION");

        network_ptr->PostRequest(url_, auth_line,
                                 [this](const Error& error_state)
                                 {onGetRegistrationResponse(error_state);});
    }
    void onGetRegistrationResponse(const Error& error_state) override {
        std::cout << "In response" <<std::endl;
        std::cout << error_state.type << ' ' <<error_state.message << std::endl;
                if (error_state.type == "0") {
                     registration_handler_ptr->passToMain(error_state.message);
                } else {
                    registration_handler_ptr->sendError(error_state);
                }
    };
    void setUser(const std::string user) override {};
    std::string getUrl() override {};
    void setUrl(const std::string url) override {
        url_ = url;
    };
private:
    ptr_to_inetwork network_ptr;
    ptr_to_registration_handler registration_handler_ptr;
    RegInput reg_params_;
    std::string url_;
};

#endif // REGISTRATIONWINDOW_NETWORK_H
