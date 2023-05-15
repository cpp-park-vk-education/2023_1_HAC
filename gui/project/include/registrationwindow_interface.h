#ifndef REGISTRATIONWINDOW_INTERFACE_H
#define REGISTRATIONWINDOW_INTERFACE_H

#include "../include/usecase_registrationwindow_interface.h"

class IHandlerRegistrationWindow;
using ptr_to_registration_handler = std::shared_ptr<IHandlerRegistrationWindow>;

class IRegistrationWindow {
public:
    ~IRegistrationWindow() = default;
    virtual void setRegistrationWindowHandler(ptr_to_registration_handler reg_handler_ptr) = 0;
    virtual void showErrorMessage() = 0;
    virtual void createErrorMessage(const Error& error_message) = 0;
    virtual std::string getLogin() = 0;
    virtual std::string getPassword() = 0;
    virtual std::string getPasswordConfirm() = 0;
    virtual std::string getEmail() = 0;
};

#endif // REGISTRATIONWINDOW_INTERFACE_H
