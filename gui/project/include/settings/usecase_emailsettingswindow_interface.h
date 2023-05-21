#ifndef USECASE_EMAILSETTINGSWINDOW_INTERFACE_H
#define USECASE_EMAILSETTINGSWINDOW_INTERFACE_H

#include <memory>

#include "emailsettingswindow_interface.h"
#include "usersettingswindow_network_interface.h"
#include "../../include/guicontroller_interface.h"


#include "../../include/ionetwork_interface.h"

class IWindowManager;
using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

class IEmailSettingsWindow;
using ptr_to_iemailsettings_window = std::shared_ptr<IEmailSettingsWindow>;

class INetworkUserSettingsWindow;
using ptr_to_isettings_network = std::shared_ptr<INetworkUserSettingsWindow>;

class IHandlerEmailSettingsWindow {
public:
    ~IHandlerEmailSettingsWindow() = default;
    virtual void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) = 0;
    virtual void setEmailSettingsWindow(ptr_to_iemailsettings_window settings_wind_ptr) = 0;
    virtual void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) = 0;
    virtual ptr_to_iemailsettings_window getEmailSettingsWindow() = 0;

    virtual void sendError(const Error& error_message) = 0;
    virtual void passToMain() = 0;
    virtual void ConfirmHandler(const std::string& new_email) = 0;
    virtual std::string getUser() = 0;
    virtual void setUser(const std::string& user) = 0;
};

#endif // USECASE_EMAILSETTINGSWINDOW_INTERFACE_H
