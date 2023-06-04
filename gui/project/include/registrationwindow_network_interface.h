#ifndef REGISTRATIONWINDOW_NETWORK_INTERFACE_H
#define REGISTRATIONWINDOW_NETWORK_INTERFACE_H

#include <memory>
#include <string>

#include "../include/ionetwork_interface.h"
#include "../include/usecase_registrationwindow_interface.h"

struct RegInput {
    std::string email;
    std::string login;
    std::string password;
};

class IHandlerRegistrationWindow;
using ptr_to_registration_handler = std::shared_ptr<IHandlerRegistrationWindow>;

class IONetworkInterface;
using ptr_to_inetwork = std::shared_ptr<IONetworkInterface>;

class INetworkRegistrationWindow {
public:
    virtual void setRegistrationHandler(ptr_to_registration_handler reg_handler_ptr) = 0;
    virtual void setRegistrationNetwork (ptr_to_inetwork net_ptr) = 0;
    virtual void getRegistration(const RegInput& reg_params) = 0;
    virtual void onGetRegistrationResponse(const Error& error_state) = 0;
    virtual void setUser(const std::string user) = 0;
    virtual void setUrl(const std::string url) = 0;
};

#endif // REGISTRATIONWINDOW_NETWORK_INTERFACE_H
