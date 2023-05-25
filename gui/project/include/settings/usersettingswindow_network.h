#ifndef USERSETTINGSWINDOW_NETWORK_H
#define USERSETTINGSWINDOW_NETWORK_H

#include <iostream>
#include <string>
#include <memory>

#include "usersettingswindow_network_interface.h"

class NetworkUserSettingsWindow : public INetworkUserSettingsWindow {
public:
    void setUserPasswordSettingsHandler(ptr_to_passwordsettings_handler set_passwordhandler_ptr) override;
    void setUserEmailSettingsHandler(ptr_to_emailsettings_handler set_emailhandler_ptr) override;
    
    //void setUserSettingsHandler(ptr_to_settings_handler set_handler_ptr) override;
    void setUserSettingsNetwork(ptr_to_inetwork net_ptr) override;
    void getUserPasswordSettings(const ConfirmEdit& confirm_passwords) override;
    void getUserEmailSettings(const ConfirmEdit& confirm_email) override;
    void onGetUserPasswordSettingsResponse(const Error& error_state) override;
    void onGetUserEmailSettingsResponse(const Error& error_state) override;

    void getUserExit(const std::string& user) override;
    void onGetExitResponse(const Error& error_state) {}

    void setUrl(const std::string& url) override;
    
private:
    //ptr_to_settings_handler settings_handler_ptr;
    ptr_to_passwordsettings_handler passwordsettings_handler_ptr;
    ptr_to_emailsettings_handler emailsettings_handler_ptr;
   
    ptr_to_inetwork network_ptr;
    std::string url_;
};


#endif // USERSETTINGSWINDOW_NETWORK_H
