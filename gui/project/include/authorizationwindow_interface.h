#ifndef AUTHORIZATIONWINDOW_INTERFACE_H
#define AUTHORIZATIONWINDOW_INTERFACE_H

#include "../include/usecase_authorizationwindow_interface.h"

class IHandlerAuthorizationWindow;
using ptr_to_authorization_handler =
        std::shared_ptr<IHandlerAuthorizationWindow>;

class IAuthorizationWindow {
public:
    ~IAuthorizationWindow() = default;
    virtual void setAuthorizationWindowHandler(
            ptr_to_authorization_handler auth_handler_ptr) = 0;
    virtual void showErrorMessage() = 0;
    virtual void createErrorMessage(const Error& error_message) = 0;
    virtual std::string getLogin() = 0;
    virtual std::string getPassword() = 0;
};

#endif // AUTHORIZATIONWINDOW_INTERFACE_H
