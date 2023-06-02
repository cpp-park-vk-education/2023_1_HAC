#include "../../include/settings/usersettingswindow_network.h"

void NetworkUserSettingsWindow::setUserPasswordSettingsHandler(ptr_to_passwordsettings_handler set_handler_ptr) {
    passwordsettings_handler_ptr = set_handler_ptr;
}

void NetworkUserSettingsWindow::setUserEmailSettingsHandler(ptr_to_emailsettings_handler set_handler_ptr) {
    emailsettings_handler_ptr = set_handler_ptr;
}


void NetworkUserSettingsWindow::setUserSettingsNetwork(ptr_to_inetwork net_ptr) {
    network_ptr = net_ptr;
}

void NetworkUserSettingsWindow::getUserPasswordSettings(const ConfirmEdit& confirm_passwords) {
    std::string setting_line = confirm_passwords.user_name + "\t" + confirm_passwords.old_password + 
                "\t" + confirm_passwords.new_password;
    network_ptr->setConfig("CHANGE_USER_PASSWORD_SETTINGS");
    network_ptr->PostRequest(url_, setting_line,
                                [this](const Error& error_state)
                                {onGetUserPasswordSettingsResponse
                                        (error_state);});
}

void NetworkUserSettingsWindow::getUserEmailSettings(const ConfirmEdit& confirm_email) {
    std::string setting_line = confirm_email.user_name + "\t" + confirm_email.new_email + '\t' + confirm_email.old_password;
    network_ptr->setConfig("CHANGE_USER_EMAIL_SETTINGS");
    network_ptr->PostRequest(url_, setting_line,
                                [this](const Error& error_state)
                                {onGetUserEmailSettingsResponse
                                        (error_state);});
}


void NetworkUserSettingsWindow::onGetUserPasswordSettingsResponse(const Error& error_state) {
    if (error_state.type == "0") {
        passwordsettings_handler_ptr->passToMain();
    } else {
        passwordsettings_handler_ptr->sendError(error_state);
    }
}

void NetworkUserSettingsWindow::onGetUserEmailSettingsResponse(const Error& error_state) {
    if (error_state.type == "0") {
        emailsettings_handler_ptr->passToMain();
        
    } else {
        emailsettings_handler_ptr->sendError(error_state);
    }
}

void NetworkUserSettingsWindow::setUrl(const std::string& url) {
    url_ = url;
}

void NetworkUserSettingsWindow::getUserExit(const std::string& user) {
    network_ptr->setConfig("DELETECOOKIE");
    network_ptr->PostRequest(url_, user,
                                [this](const Error& error_state)
                                {onGetExitResponse(error_state);});
}



