#ifndef USERSETTINGSWINDOW_INTERFACE_H
#define USERSETTINGSWINDOW_INTERFACE_H

#include <memory>
#include "usecase_usersettingswindow_interface.h"
#include "../ionetwork_interface.h"

class IHandlerUserSettingsWindow;
using ptr_to_settings_handler = std::shared_ptr<IHandlerUserSettingsWindow>;

class IUserSettingsWindow {
public:
    ~IUserSettingsWindow() = default;
    virtual void setUserSettingsWindowHandler(ptr_to_settings_handler handler_user_set_ptr) = 0;

};

#endif // USERSETTINGSWINDOW_INTERFACE_H
