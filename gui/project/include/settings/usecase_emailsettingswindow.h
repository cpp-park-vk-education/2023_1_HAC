#ifndef USECASE_EMAILSETTINGSWINDOW_H
#define USECASE_EMAILSETTINGSWINDOW_H

#include <string>
#include <iostream>

#include "usecase_emailsettingswindow_interface.h"

class UseCaseEmailSettingsWindow : public IHandlerEmailSettingsWindow {
public:
    ~UseCaseEmailSettingsWindow();
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override;
    void setEmailSettingsWindow(ptr_to_iemailsettings_window user_set_ptr) override;
    void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) override;
    ptr_to_iemailsettings_window getEmailSettingsWindow() override;
    void sendError(const Error& error_message) override;
    void passToMain() override;
    void ConfirmHandler(const std::string& new_email) override;
    std::string getUser() override;
    void setUser(const std::string& user) override;

private:
    ptr_to_iemailsettings_window settings_window_ptr;
    ptr_to_isettings_network settings_network_ptr;
    ptr_to_iwindow_manager window_manager_ptr;

    std::string url_;
    std::string user_;
};

#endif // USECASE_EMAILSETTINGSWINDOW_H