#include "../include/usersettingswindow_network.h"

void NetworkUserSettingsWindow::setUserSettingsHandler(ptr_to_settings_handler set_handler_ptr) {
    settings_handler_ptr = set_handler_ptr;
}

void NetworkUserSettingsWindow::setUserSettingsNetwork(ptr_to_inetwork net_ptr) {
    network_ptr = net_ptr;
}

void NetworkUserSettingsWindow::getUserSettings(const ConfirmEdit& confirm_passwords) {
    std::cout << "here I am in UserSetting" << std::endl;
    std::string setting_line = confirm_passwords.user_name + "\t" + confirm_passwords.old_password + 
                "\t" + confirm_passwords.new_password;
    network_ptr->setConfig("CHANGE_USER_SETTINGS");
    network_ptr->PostRequest(url_, setting_line,
                                [this](const Error& error_state)
                                {onGetUserSettingsResponse
                                        (error_state);});
}

void NetworkUserSettingsWindow::onGetUserSettingsResponse(const Error& error_state) {
    std::cout << "In settings Response" <<std::endl;
    std::cout << error_state.type << ' ' << error_state.message << std::endl;
    if (error_state.type == "0") {
        settings_handler_ptr->passToMain();
    } else {
        settings_handler_ptr->sendError(error_state);
    }
}

void NetworkUserSettingsWindow::setUrl(const std::string& url) {
    url_ = url;
}



