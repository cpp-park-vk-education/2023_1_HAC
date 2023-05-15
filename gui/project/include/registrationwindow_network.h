#ifndef REGISTRATIONWINDOW_NETWORK_H
#define REGISTRATIONWINDOW_NETWORK_H

//#include "project/include/ionetwork_interface.h"
//#include "../include/usecase_registrationwindow_interface.h"
#include "../include/registrationwindow_network_interface.h"

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
    };
    void onGetRegistrationResponse(const Error& error_state) override {};
    void setUser(const std::string user) override {};
    std::string getUrl() override {};
    void setUrl(const std::string url) override {};
private:
    ptr_to_inetwork network_ptr;
    ptr_to_registration_handler registration_handler_ptr;
};

#endif // REGISTRATIONWINDOW_NETWORK_H
