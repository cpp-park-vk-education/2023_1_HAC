#ifndef USECASE_PASSWORDSETTINGSWINDOW_H
#define USECASE_PASSWORDSETTINGSWINDOW_H

#include <string>
#include <iostream>

#include "usecase_passwordsettingswindow_interface.h"

class UseCasePasswordSettingsWindow : public IHandlerPasswordSettingsWindow {
public:
    ~UseCasePasswordSettingsWindow();
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override;
    void setPasswordSettingsWindowHandler(ptr_to_ipasswordsettings_window user_set_ptr) override;
    void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) override;
    ptr_to_ipasswordsettings_window getPasswordSettingsWindow() override;
    void sendError(const Error& error_message) override;
    void passToMain() override;
    void ConfirmHandler(const std::string& old_password,
                                 const std::string& new_password, const std::string& repeat_password) override;
    std::string getUser() override;
    void setUser(const std::string& user) override;

private:
    ptr_to_ipasswordsettings_window settings_window_ptr;
    ptr_to_isettings_network settings_network_ptr;
    ptr_to_iwindow_manager window_manager_ptr;

    std::string url_;
    std::string user_;
};

#endif // USECASE_PASSWORDSETTINGSWINDOW_H