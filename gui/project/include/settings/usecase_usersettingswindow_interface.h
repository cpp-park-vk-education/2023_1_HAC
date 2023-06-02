#ifndef USECASE_USERSETTINGSWINDOW_INTERFACE_H
#define USECASE_USERSETTINGSWINDOW_INTERFACE_H

#include <memory>

#include "usersettingswindow_interface.h"
#include "usersettingswindow_network_interface.h"
#include "../../include/guicontroller_interface.h"
#include "../../include/ionetwork_interface.h"

class IWindowManager;
using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

class IUserSettingsWindow;
using ptr_to_isettings_window = std::shared_ptr<IUserSettingsWindow>;

class INetworkUserSettingsWindow;
using ptr_to_isettings_network = std::shared_ptr<INetworkUserSettingsWindow>;

class IPasswordSettingsWindow;
using ptr_to_ipassword_settings_window = std::shared_ptr<IPasswordSettingsWindow>;

class IEmailSettingsWindow;
using ptr_to_iemail_settings_window = std::shared_ptr<IEmailSettingsWindow>;


class IHandlerUserSettingsWindow {
public:
    ~IHandlerUserSettingsWindow() = default;
    virtual void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) = 0;
    virtual void setUserSettingsWindow(
            ptr_to_isettings_window settings_wind_ptr) = 0; 

    virtual void passToMain() = 0;
    virtual void openPasswordWindow() = 0;
    virtual void openEmailWindow() = 0;
    virtual void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) = 0;
    virtual void userExitHandler() = 0;
    virtual std::string getUser() = 0;
    virtual void setUser(const std::string& user) = 0;
};



#endif // USECASE_USERSETTINGSWINDOW_H
