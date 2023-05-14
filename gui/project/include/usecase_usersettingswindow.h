#ifndef USECASE_USERSETTINGSWINDOW_H
#define USECASE_USERSETTINGSWINDOW_H

#include <string>

#include "../include/usecase_usersettingswindow_interface.h"

class UseCaseUserSettingsWindow : public IHandlerUserSettingsWindow {
public:
    ~UseCaseUserSettingsWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setUserSettingsWindow(
            ptr_to_isettings_window user_set_ptr) override {
        settings_window_ptr = user_set_ptr;
    }
    void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr)
    override {}
    ptr_to_isettings_window getUserSettingsWindow() override {
        return settings_window_ptr;
    }
    void sendError(const Error& error_message) override {}
    void passToMain() override {}
    void ConfirmHandler(const std::string& old_password,
                                 const std::string& new_password) override {}
    std::string getUser() override {}
    std::string getUrl() override {}
    void setUrl(const std::string& url) override {}
    void setUser(const std::string& user) override {}
private:
    ptr_to_isettings_window settings_window_ptr;
    ptr_to_isettings_network settings_network_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
};

#endif // USECASE_USERSETTINGSWINDOW_H
