#ifndef USECASE_REGISTRATIONWINDOW_INTERFACE_H
#define USECASE_REGISTRATIONWINDOW_INTERFACE_H

#include <memory>

#include "../include/registrationwindow_interface.h"
#include "../include/registrationwindow_network_interface.h"
#include "../include/guicontroller_interface.h"

//class IWindowManager;
using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

class IRegistrationWindow;
using ptr_to_iregistration_window = std::shared_ptr<IRegistrationWindow>;

class INetworkRegistrationWindow;
using ptr_to_iregistration_network =
        std::shared_ptr<INetworkRegistrationWindow>;

class IHandlerRegistrationWindow {
public:
    ~IHandlerRegistrationWindow() = default;
    virtual void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) = 0;
    virtual void setRegistrationWindow(ptr_to_iregistration_window reg_window_ptr) = 0;
    virtual void setRegistrationNetwork(ptr_to_iregistration_network reg_net_ptr) = 0;
    virtual ptr_to_iregistration_window getRegistrationWindow() = 0;
    virtual void regHandler(const std::string& email,
                            const std::string& login,
                            const std::string& password,
                            const std::string& pass_confirm) = 0;
    virtual void passToMain() = 0;
    virtual void sendError(const Error& error_message) = 0;
    virtual void setUser(const std::string& user) = 0;
    virtual std::string getUrl() = 0;
    virtual void setUrl(const std::string& url) = 0;
};

#endif // USECASE_REGISTRATIONWINDOW_INTERFACE_H
