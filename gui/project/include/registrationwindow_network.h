#ifndef REGISTRATIONWINDOW_NETWORK_H
#define REGISTRATIONWINDOW_NETWORK_H

//#include "project/include/ionetwork_interface.h"
//#include "../include/usecase_registrationwindow_interface.h"
#include "../include/registrationwindow_network_interface.h"

class NetworkRegistrationWindow : public INetworkRegistrationWindow {
    void setRegistrationHandler(ptr_to_registration_handler reg_handler_ptr) override {};
    void setRegistrationNetwork(ptr_to_inetwork net_ptr) override {};
    void getRegistration(const RegInput& reg_params) override {};
    void onGetRegistrationResponse(const Error& error_state) override {};
private:
    ptr_to_inetwork network_ptr;
    ptr_to_registration_handler registration_handler_ptr;
};

#endif // REGISTRATIONWINDOW_NETWORK_H
