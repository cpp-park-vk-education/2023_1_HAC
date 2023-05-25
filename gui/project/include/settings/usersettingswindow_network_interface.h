#ifndef USERSETTINGSWINDOW_NETWORK_INTERFACE_H
#define USERSETTINGSWINDOW_NETWORK_INTERFACE_H


#include <memory>

struct ConfirmEdit {
    std::string old_password;
    std::string new_password;
    std::string user_name;
    std::string new_email;
};

#include "passwordsettingswindow_interface.h"
#include "emailsettingswindow_interface.h"
#include "../ionetwork_interface.h"

class IHandlerPasswordSettingsWindow;
using ptr_to_passwordsettings_handler = std::shared_ptr<IHandlerPasswordSettingsWindow>;

class IHandlerEmailSettingsWindow;
using ptr_to_emailsettings_handler = std::shared_ptr<IHandlerEmailSettingsWindow>;

class IONetworkInterface;
using ptr_to_inetwork = std::shared_ptr<IONetworkInterface>;

class INetworkUserSettingsWindow {
public:
    virtual void setUserPasswordSettingsHandler(ptr_to_passwordsettings_handler set_passwordhandler_ptr) = 0;
    virtual void setUserEmailSettingsHandler(ptr_to_emailsettings_handler set_emailhandler_ptr) = 0;
    virtual void setUserSettingsNetwork(ptr_to_inetwork net_ptr) = 0;
    virtual void getUserPasswordSettings(const ConfirmEdit& confirm_passwords) = 0;
    virtual void getUserEmailSettings(const ConfirmEdit& confirm_email) = 0;
    //virtual void onGetUserSettingsResponse(const Error& error_state) = 0;
    virtual void onGetUserPasswordSettingsResponse(const Error& error_state) = 0;
    virtual void onGetUserEmailSettingsResponse(const Error& error_state) = 0;
    virtual void getUserExit(const std::string& user) = 0;
    virtual void setUrl(const std::string& url) = 0;
};

#endif // USERSETTINGSWINDOW_NETWORK_INTERFACE_H
