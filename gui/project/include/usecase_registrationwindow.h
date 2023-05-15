#ifndef USECASE_REGISTRATIONWINDOW_H
#define USECASE_REGISTRATIONWINDOW_H

//#include "../include/registrationwindow_interface.h"
#include "../include/usecase_registrationwindow_interface.h"
//#include "../include/registrationwindow_network_interface.h"

class UseCaseRegistrationWindow : public IHandlerRegistrationWindow {
public:
    ~UseCaseRegistrationWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setRegistrationWindow(ptr_to_iregistration_window reg_window_ptr) override {
        registration_window_ptr = reg_window_ptr;
    }
    void setRegistrationNetwork(ptr_to_iregistration_network reg_net_ptr) override {}
    ptr_to_iregistration_window getRegistrationWindow() override {
        return registration_window_ptr;
    }
    void regHandler(const std::string& login,
                    const std::string& password) override {}
    void passToMain() override {}
    void sendError(const Error& error_message) override {}
    void setUser(const std::string& user) override {}
    std::string getUrl() override {}
    void setUrl(const std::string& url) override {}
private:
    ptr_to_iregistration_network registration_network_ptr;
    ptr_to_iregistration_window registration_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
};

#endif // USECASE_REGISTRATIONWINDOW_H
