#ifndef USERSETTINGSWINDOW_NETWORK_INTERFACE_H
#define USERSETTINGSWINDOW_NETWORK_INTERFACE_H

//#include <memory>

struct ConfirmEdit {
    std::string old_password;
    std::string new_password;
};

#include "../include/usersettingswindow_interface.h"
#include "../include/ionetwork_interface.h"

class IHandlerUserSettingsWindow;
using ptr_to_settings_handler = std::shared_ptr<IHandlerUserSettingsWindow>;

class IONetworkInterface;
using ptr_to_inetwork = std::shared_ptr<IONetworkInterface>;

class INetworkUserSettingsWindow {
    virtual void setUserSettingsHandler(ptr_to_settings_handler set_handler_ptr) = 0;
    virtual void setUserSettingsNetwork(ptr_to_inetwork net_ptr) = 0;
    virtual void getUserSettings(const ConfirmEdit& confirm_passwords) = 0;
    virtual void onGetUserSettingsResponse(const Error& error_state) = 0;
};

#endif // USERSETTINGSWINDOW_NETWORK_INTERFACE_H
