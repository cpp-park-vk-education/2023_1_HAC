#ifndef USECASE_REGISTRATIONWINDOW_H
#define USECASE_REGISTRATIONWINDOW_H

//#include "../include/registrationwindow_interface.h"
#include "../include/usecase_registrationwindow_interface.h"
//#include "../include/registrationwindow_network_interface.h"

#include <string>

class UseCaseRegistrationWindow : public IHandlerRegistrationWindow {
public:
    ~UseCaseRegistrationWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override;
    void setRegistrationWindow(ptr_to_iregistration_window reg_window_ptr)
    override;
    void setRegistrationNetwork(ptr_to_iregistration_network reg_net_ptr)
    override;
    ptr_to_iregistration_window getRegistrationWindow() override;
    void regHandler(const std::string& email,
                    const std::string& login,
                    const std::string& password,
                    const std::string& pass_confirm) override;
    void passToMain(const std::string& user) override;
    void sendError(const Error& error_message) override;

    void setUser(const std::string& user) override {}
    std::string getUrl() override {}
    void setUrl(const std::string& url) override {}

    void openAuthorizationWindow() override;

private:
    ptr_to_iregistration_network registration_network_ptr;
    ptr_to_iregistration_window registration_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
};

#endif // USECASE_REGISTRATIONWINDOW_H
