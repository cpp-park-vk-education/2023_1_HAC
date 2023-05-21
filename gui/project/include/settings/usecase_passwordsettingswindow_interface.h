#ifndef USECASE_PASSWORDSETTINGSWINDOW_INTERFACE_H
#define USECASE_PASSWORDSETTINGSWINDOW_INTERFACE_H

#include "passwordsettingswindow_interface.h"
#include "usersettingswindow_network_interface.h"
#include "../../include/guicontroller_interface.h"

#include<memory>

#include "../../include/ionetwork_interface.h"

class IWindowManager;
using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

class IPasswordSettingsWindow;
using ptr_to_ipasswordsettings_window = std::shared_ptr<IPasswordSettingsWindow>;

class INetworkUserSettingsWindow;
using ptr_to_isettings_network = std::shared_ptr<INetworkUserSettingsWindow>;

class IHandlerPasswordSettingsWindow {
public:
    ~IHandlerPasswordSettingsWindow() = default;
    virtual void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) = 0;
    virtual void setPasswordSettingsWindowHandler(
            ptr_to_ipasswordsettings_window settings_wind_ptr) = 0;
    virtual void setUserSettingsNetwork(
            ptr_to_isettings_network settings_net_ptr) = 0;
    virtual ptr_to_ipasswordsettings_window getPasswordSettingsWindow() = 0;
    virtual void sendError(const Error& error_message) = 0;
    virtual void passToMain() = 0;
    virtual void ConfirmHandler(const std::string& old_password,

                                         const std::string& new_password, const std::string& repeat_password) = 0;
    virtual std::string getUser() = 0;
    virtual void setUser(const std::string& user) = 0;
};

#endif // USECASE_PASSWORDSETTINGSWINDOW_INTERFACE_H
