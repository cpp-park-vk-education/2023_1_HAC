#ifndef USECASE_AUTHORIZATIONWINDOW_INTERFACE_H
#define USECASE_AUTHORIZATIONWINDOW_INTERFACE_H

#include <memory>

#include "../include/ionetwork_interface.h"
#include "../include/authorizationwindow_interface.h"
#include "../include/authorizationwindow_network_interface.h"
#include "../include/guicontroller_interface.h"

//class IWindowManager;
using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

class IAuthorizationWindow;
using ptr_to_iauthorization_window = std::shared_ptr<IAuthorizationWindow>;

class INetworkAuthorizationWindow;
using ptr_to_iauthorization_network =
        std::shared_ptr<INetworkAuthorizationWindow>;

class IHandlerAuthorizationWindow {
public:
    ~IHandlerAuthorizationWindow() = default;
    virtual void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) = 0;
    virtual void setAuthorizationWindow(
            ptr_to_iauthorization_window auth_window_ptr) = 0;
    virtual ptr_to_iauthorization_window getAuthorizationWindow() = 0;
    virtual void setAuthorizationNetwork(
            ptr_to_iauthorization_network auth_network_ptr) = 0;
    virtual void authHandler(const std::string& login,
                             const std::string& password) = 0;
    virtual void passToMain(const std::string& user) = 0;
    virtual void sendError(const Error& error_message) = 0;
    virtual void setUser(const std::string& user) = 0;
    virtual std::string getUser() = 0;
    //virtual std::string getUrl() = 0;
    //virtual void setUrl(const std::string& url) = 0;
    virtual void check_cookie() = 0;
    virtual void openRegistrationWindow() = 0;
    virtual void checked_cookie(const std::string& user, const std::string& status) = 0;
};

#endif // USECASE_AUTHORIZATIONWINDOW_INTERFACE_H
