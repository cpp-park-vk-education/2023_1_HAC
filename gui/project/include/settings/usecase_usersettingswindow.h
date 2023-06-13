#ifndef USECASE_USERSETTINGSWINDOW_H
#define USECASE_USERSETTINGSWINDOW_H

#include <string>
#include <iostream>

#include "usecase_usersettingswindow_interface.h"

class UseCaseUserSettingsWindow : public IHandlerUserSettingsWindow {
public:
    ~UseCaseUserSettingsWindow();
    
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override;
    void setUserSettingsWindow(
            ptr_to_isettings_window user_set_ptr) override;

    void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) override;
    void userExitHandler() override;
    void openPasswordWindow() override;
    void openEmailWindow() override;
    void passToMain() override;
    std::string getUser() override;
    void setUser(const std::string& user) override;

private:
    ptr_to_isettings_window settings_window_ptr;
    ptr_to_isettings_network settings_network_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
    std::string user_;
};

#endif // USECASE_USERSETTINGSWINDOW_H
