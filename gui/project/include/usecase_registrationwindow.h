#ifndef USECASE_REGISTRATIONWINDOW_H
#define USECASE_REGISTRATIONWINDOW_H

//#include "../include/registrationwindow_interface.h"
#include "../include/usecase_registrationwindow_interface.h"
//#include "../include/registrationwindow_network_interface.h"

class UseCaseRegistrationWindow : public IHandlerRegistrationWindow {
public:
    ~UseCaseRegistrationWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {};
    void setRegistrationWindow(ptr_to_iregistration_window reg_window_ptr) override {};
    void setRegistrationNetwork(ptr_to_iregistration_network reg_net_ptr) override {};
    void regHandler(const std::string& login,
                    const std::string& password) override {};
    void sendError(const Error& error_message) override {};
private:
    ptr_to_iregistration_network registration_network_ptr;
    ptr_to_iregistration_window registration_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
};

#endif // USECASE_REGISTRATIONWINDOW_H
