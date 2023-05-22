#ifndef EMAILSETTINGSWINDOW_INTERFACE_H
#define EMAILSETTINGSWINDOW_INTERFACE_H

#include <memory>
#include "usecase_emailsettingswindow_interface.h"
#include "../ionetwork_interface.h"

class IHandlerEmailSettingsWindow;
using ptr_to_emailsettings_handler = std::shared_ptr<IHandlerEmailSettingsWindow>;

class IEmailSettingsWindow {
public:
    ~IEmailSettingsWindow() = default;
    virtual void setEmailSettingsWindowHandler(
            ptr_to_emailsettings_handler handler_user_set_ptr) = 0;
    virtual void showErrorMessage() = 0;
    virtual void createErrorMessage(const Error& error_message) = 0;
    virtual std::string getNewEmail() = 0;
    virtual std::string getPassword() = 0;
};

#endif // EMAILSETTINGSWINDOW_INTERFACE_H