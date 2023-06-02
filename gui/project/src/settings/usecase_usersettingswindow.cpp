#include "../../include/settings/usecase_usersettingswindow.h"


UseCaseUserSettingsWindow::~UseCaseUserSettingsWindow() {
};

void UseCaseUserSettingsWindow::setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}

void UseCaseUserSettingsWindow::setUserSettingsWindow(ptr_to_isettings_window user_set_ptr) {
    settings_window_ptr = user_set_ptr;
}

void UseCaseUserSettingsWindow::passToMain() {
    window_manager_ptr->openMainWindow();
}

void UseCaseUserSettingsWindow::openEmailWindow() {
    window_manager_ptr->openEmailSettingsWindow();
}

void UseCaseUserSettingsWindow::openPasswordWindow() {
    window_manager_ptr->openPasswordSettingsWindow();
}

std::string UseCaseUserSettingsWindow::getUser() {
    return user_;
}

void UseCaseUserSettingsWindow::setUser(const std::string& user) {
    user_ = user;
}

void  UseCaseUserSettingsWindow::setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) {
    settings_network_ptr = settings_net_ptr;
}

void UseCaseUserSettingsWindow::userExitHandler() {
    if (user_.empty()) {
        setUser(window_manager_ptr->getUser());
    }
    
    settings_network_ptr->getUserExit(user_);
}
