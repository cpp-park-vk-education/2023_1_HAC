#include "../include/registrationwindow_network.h"

#include <iostream>

void NetworkRegistrationWindow::setRegistrationHandler(
        ptr_to_registration_handler reg_handler_ptr) {
    registration_handler_ptr = reg_handler_ptr;
}

void NetworkRegistrationWindow::setRegistrationNetwork(
        ptr_to_inetwork net_ptr) {
    network_ptr = net_ptr;
}

void NetworkRegistrationWindow::getRegistration(const RegInput& reg_params) {
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

void NetworkRegistrationWindow::onGetRegistrationResponse(
        const Error& error_state) {
    std::cout << "In response" <<std::endl;
    std::cout << error_state.type << ' ' <<error_state.message << std::endl;
    if (error_state.type == "0") {
    registration_handler_ptr->passToMain(error_state.message);
    } else {
    registration_handler_ptr->sendError(error_state);
    }
}

void NetworkRegistrationWindow::setUrl(const std::string url) {
url_ = url;
}
