#ifndef PASSWORDSETTINGSWINDOW_INTERFACE_H
#define PASSWORDSETTINGSWINDOW_INTERFACE_H

#include <memory>
#include "usecase_passwordsettingswindow_interface.h"
#include "../ionetwork_interface.h"

class IHandlerPasswordSettingsWindow;
using ptr_to_passwordsettings_handler = std::shared_ptr<IHandlerPasswordSettingsWindow>;

class IPasswordSettingsWindow {
public:
    ~IPasswordSettingsWindow() = default;
    virtual void setPasswordSettingsWindowHandler(
            ptr_to_passwordsettings_handler handler_user_set_ptr) = 0;
    virtual void showErrorMessage() = 0;
    virtual void createErrorMessage(const Error& error_message) = 0;
    virtual std::string getOldPassword() = 0;
    virtual std::string getNewPassword() = 0;
    virtual std::string getRepeatPassword() = 0;

};

#endif // PASSWORDSETTINGSWINDOW_INTERFACE_H
