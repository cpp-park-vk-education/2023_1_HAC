#include "../../include/settings/usecase_passwordsettingswindow.h"


UseCasePasswordSettingsWindow::~UseCasePasswordSettingsWindow() {
};

void UseCasePasswordSettingsWindow::setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}

void UseCasePasswordSettingsWindow::setPasswordSettingsWindowHandler(ptr_to_ipasswordsettings_window user_set_ptr) {
    settings_window_ptr = user_set_ptr;
}

void UseCasePasswordSettingsWindow::setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) {
    settings_network_ptr = settings_net_ptr;
}

ptr_to_ipasswordsettings_window UseCasePasswordSettingsWindow::getPasswordSettingsWindow() {
    return settings_window_ptr;
}

void UseCasePasswordSettingsWindow::sendError(const Error& error_message) {
    settings_window_ptr->createErrorMessage(error_message);
    settings_window_ptr->showErrorMessage();
}

void UseCasePasswordSettingsWindow::passToMain() {
    window_manager_ptr->openUserSettingsWindow();
}

void UseCasePasswordSettingsWindow::ConfirmHandler(const std::string& old_password, 
                const std::string& new_password, const std::string&  repeat_password) {
    if (old_password ==  new_password) {
        std::cout << "bad" << std::endl;
        Error error_message;
        error_message.type = "BadNewPassword";
        error_message.message = "The old password cannot be the new password!";
        settings_window_ptr->createErrorMessage(error_message);
        settings_window_ptr->showErrorMessage();
        return;
    }       

    if (new_password != repeat_password) {
        std::cout << "bad" << std::endl;
        Error error_message;
        error_message.type = "BadNewPassword";
        error_message.message = "Passwords don't match!";
        settings_window_ptr->createErrorMessage(error_message);
        settings_window_ptr->showErrorMessage();
        return;
    }

    if (new_password.empty() || old_password.empty()) {
        std::cout << "bad" << std::endl;
        Error error_message;
        error_message.type = "BadPassword";
        error_message.message = "Password is null!";
        settings_window_ptr->createErrorMessage(error_message);
        settings_window_ptr->showErrorMessage();
        return;        
    }

    ConfirmEdit user;
    user.old_password = old_password;
    user.new_password = new_password;    
    if (user_ == "") {
        setUser(window_manager_ptr->getUser());
    } 

    user.user_name = user_;
    settings_network_ptr->getUserPasswordSettings(user);
}

std::string UseCasePasswordSettingsWindow::getUser() {
    return user_;
}

void UseCasePasswordSettingsWindow::setUser(const std::string& user) {
    user_ = user;
}
