#ifndef USERSETTINGSWINDOW_NETWORK_H
#define USERSETTINGSWINDOW_NETWORK_H

#include <string>
#include <memory>

#include "../include/usersettingswindow_network_interface.h"

class NetworkUserSettingsWindow : public INetworkUserSettingsWindow {
public:
    void setUserSettingsHandler(ptr_to_settings_handler set_handler_ptr) override {}
    void setUserSettingsNetwork(ptr_to_inetwork net_ptr) override {}
    void getUserSettings(const ConfirmEdit& confirm_passwords) override {}
    void onGetUserSettingsResponse(const Error& error_state) override {}
    void setUrl(const std::string& url) override {}
private:
    ptr_to_settings_handler settings_handler_ptr;
    ptr_to_inetwork network_ptr;
};

#endif // USERSETTINGSWINDOW_NETWORK_H
