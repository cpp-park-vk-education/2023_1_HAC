#include "../include/usecase_usersettingswindow.h"


UseCaseUserSettingsWindow::~UseCaseUserSettingsWindow() {
};

void UseCaseUserSettingsWindow::setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}

void UseCaseUserSettingsWindow::setUserSettingsWindow(ptr_to_isettings_window user_set_ptr) {
    settings_window_ptr = user_set_ptr;
}

void UseCaseUserSettingsWindow::setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) {
    settings_network_ptr = settings_net_ptr;
}

ptr_to_isettings_window UseCaseUserSettingsWindow::getUserSettingsWindow() {
    return settings_window_ptr;
}

void UseCaseUserSettingsWindow::sendError(const Error& error_message) {
    settings_window_ptr->createErrorMessage(error_message);
    settings_window_ptr->showErrorMessage();
}

void UseCaseUserSettingsWindow::passToMain() {
    window_manager_ptr->openMainWindow();
}

void UseCaseUserSettingsWindow::ConfirmHandler(const std::string& old_password, 
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

    if (new_password == "") {
        std::cout << "bad" << std::endl;
        Error error_message;
        error_message.type = "BadNewPassword";
        error_message.message = "Password is null!";
        settings_window_ptr->createErrorMessage(error_message);
        settings_window_ptr->showErrorMessage();
        return;
    }

    if (old_password.empty()) {
        std::cout << "bad" << std::endl;
        Error error_message;
        error_message.type = "BadOldPassword";
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
    settings_network_ptr->getUserSettings(user);
}

std::string UseCaseUserSettingsWindow::getUser() {
    return user_;
}

// std::string UseCaseUserSettingsWindow::getUrl() {
//     return url_;
// }

// void UseCaseUserSettingsWindow::setUrl(const std::string& url) {
//     url_ = url;
// }


void UseCaseUserSettingsWindow::setUser(const std::string& user) {
    user_ = user;
}
