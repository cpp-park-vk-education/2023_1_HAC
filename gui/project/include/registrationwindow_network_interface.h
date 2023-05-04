#ifndef REGISTRATIONWINDOW_NETWORK_INTERFACE_H
#define REGISTRATIONWINDOW_NETWORK_INTERFACE_H

#include <memory>

struct RegInput {
    std::string login;
    std::string password;
};

#include "../include/ionetwork_interface.h"
#include "../include/usecase_registrationwindow_interface.h"

class IHandlerRegistrationWindow;
using ptr_to_registration_handler = std::shared_ptr<IHandlerRegistrationWindow>;

class IONetworkInterface;
using ptr_to_inetwork = std::shared_ptr<IONetworkInterface>;

class INetworkRegistrationWindow {
    virtual void setRegistrationHandler(ptr_to_registration_handler reg_handler_ptr) = 0;
    virtual void setRegistrationNetwork (ptr_to_inetwork net_ptr) = 0;
    virtual void getRegistration(const RegInput& reg_params) = 0;
    virtual void onGetRegistrationResponse(const Error& error_state) = 0;
};

#endif // REGISTRATIONWINDOW_NETWORK_INTERFACE_H
