#include "../../include/settings/usecase_usersettingswindow.h"


UseCaseUserSettingsWindow::~UseCaseUserSettingsWindow() {
};

void UseCaseUserSettingsWindow::setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}

void UseCaseUserSettingsWindow::setUserSettingsWindow(ptr_to_isettings_window user_set_ptr) {
    settings_window_ptr = user_set_ptr;
}

// void UseCaseUserSettingsWindow::setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) {
//     settings_network_ptr = settings_net_ptr;
// }

// ptr_to_isettings_window UseCaseUserSettingsWindow::getUserSettingsWindow() {
//     return settings_window_ptr;
// }

void UseCaseUserSettingsWindow::passToMain() {
    window_manager_ptr->openMainWindow();
}

void UseCaseUserSettingsWindow::OpenEmailWindow() {
    window_manager_ptr->openEmailSettingsWindow();
}

void UseCaseUserSettingsWindow::OpenPasswordWindow() {
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

void UseCaseUserSettingsWindow::UserExitHandler() {
    std::cout << "in handler exit" <<std::endl;
    if (user_.empty()) {
        setUser(window_manager_ptr->getUser());
    }
    std::cout << "user = " <<user_ <<std::endl;
    settings_network_ptr->getUserExit(user_);
}
