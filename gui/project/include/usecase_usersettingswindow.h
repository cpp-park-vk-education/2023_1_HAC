#ifndef USECASE_USERSETTINGSWINDOW_H
#define USECASE_USERSETTINGSWINDOW_H

#include <string>

#include "../include/usecase_usersettingswindow_interface.h"

class UseCaseUserSettingsWindow : public IHandlerUserSettingsWindow {
    ~UseCaseUserSettingsWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {};
    void setUserSettingsWindow(
            ptr_to_isettings_window user_set_ptr) override {};
    void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr)
    override {};
    void btnConfirmHandler(std::string confirm_line) override {}; //??????
    void sendError(const Error& error_message) override {};
    void passToMain() override {};
    void onbtnConfirmEditClicked(const std::string& old_password,
                                 const std::string& new_password) override {};
private:
    ptr_to_isettings_window settings_window_ptr;
    ptr_to_isettings_network settings_network_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
};

#endif // USECASE_USERSETTINGSWINDOW_H
