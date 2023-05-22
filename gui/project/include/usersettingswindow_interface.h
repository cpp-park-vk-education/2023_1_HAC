#ifndef USERSETTINGSWINDOW_INTERFACE_H
#define USERSETTINGSWINDOW_INTERFACE_H

#include <memory>
#include "../include/usecase_usersettingswindow_interface.h"
#include "../include/ionetwork_interface.h"

class IHandlerUserSettingsWindow;
using ptr_to_settings_handler = std::shared_ptr<IHandlerUserSettingsWindow>;

class IUserSettingsWindow {
public:
    ~IUserSettingsWindow() = default;
    virtual void setUserSettingsWindowHandler(
            ptr_to_settings_handler handler_user_set_ptr) = 0;
    virtual void showErrorMessage() = 0;
    virtual void createErrorMessage(const Error& error_message) = 0;
    virtual std::string getOldPassword() = 0;
    virtual std::string getNewPassword() = 0;
    virtual std::string getRepeatPassword() = 0;

};

#endif // USERSETTINGSWINDOW_INTERFACE_H