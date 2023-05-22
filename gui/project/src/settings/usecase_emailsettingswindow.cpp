#include "../../include/settings/usecase_emailsettingswindow.h"


UseCaseEmailSettingsWindow::~UseCaseEmailSettingsWindow() {
};

void UseCaseEmailSettingsWindow::setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}

void UseCaseEmailSettingsWindow::setEmailSettingsWindow(ptr_to_iemailsettings_window user_set_ptr) {
    settings_window_ptr = user_set_ptr;
}

void UseCaseEmailSettingsWindow::setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) {
    settings_network_ptr = settings_net_ptr;
}

ptr_to_iemailsettings_window UseCaseEmailSettingsWindow::getEmailSettingsWindow() {
    return settings_window_ptr;
}

void UseCaseEmailSettingsWindow::sendError(const Error& error_message) {
    settings_window_ptr->createErrorMessage(error_message);
    settings_window_ptr->showErrorMessage();
}

void UseCaseEmailSettingsWindow::passToMain() {
    window_manager_ptr->openUserSettingsWindow();
}

void UseCaseEmailSettingsWindow::ConfirmHandler(const std::string& new_email, const std::string& password) {

    if (new_email.empty() || password.empty()) {
        std::cout << "bad" << std::endl;
        Error error_message;
        error_message.type = "BadEmail";
        error_message.message = "Fields cannot be empty!";
        settings_window_ptr->createErrorMessage(error_message);
        settings_window_ptr->showErrorMessage();
        return;        
    }       

    ConfirmEdit user;
    user.new_email = new_email;
    user.old_password = password;
    if (user_ == "") {
        setUser(window_manager_ptr->getUser());
    } 

    user.user_name = user_;
    settings_network_ptr->getUserEmailSettings(user);
}

std::string UseCaseEmailSettingsWindow::getUser() {
    return user_;
}

void UseCaseEmailSettingsWindow::setUser(const std::string& user) {
    user_ = user;
}