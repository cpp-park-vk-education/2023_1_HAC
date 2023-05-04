#ifndef USECASE_USERSETTINGSWINDOW_INTERFACE_H
#define USECASE_USERSETTINGSWINDOW_INTERFACE_H

#include "../include/usersettingswindow_interface.h"
#include "../include/usersettingswindow_network_interface.h"
#include "../include/guicontroller_interface.h"

#include<memory>

class IWindowManager;
using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

class IUserSettingsWindow;
using ptr_to_isettings_window = std::shared_ptr<IUserSettingsWindow>;

class INetworkUserSettingsWindow;
using ptr_to_isettings_network = std::shared_ptr<INetworkUserSettingsWindow>;

class IHandlerUserSettingsWindow {
public:
    ~IHandlerUserSettingsWindow() = default;
    virtual void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) = 0;
    virtual void setUserSettingsWindow(
            ptr_to_isettings_window settings_wind_ptr) = 0;
    virtual void setUserSettingsNetwork(
            ptr_to_isettings_network settings_net_ptr) = 0;
    virtual void btnConfirmHandler(std::string confirm_line) = 0;
    virtual void sendError(const Error& error_message) = 0;
    virtual void passToMain() = 0;
    virtual void onbtnConfirmEditClicked(const std::string& old_password,
                                         const std::string& new_password) = 0;
};



#endif // USECASE_USERSETTINGSWINDOW_H
