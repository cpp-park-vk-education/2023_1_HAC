#ifndef USECASE_REGISTRATIONWINDOW_H
#define USECASE_REGISTRATIONWINDOW_H

//#include "../include/registrationwindow_interface.h"
#include "../include/usecase_registrationwindow_interface.h"
//#include "../include/registrationwindow_network_interface.h"

#include <string>

class UseCaseRegistrationWindow : public IHandlerRegistrationWindow {
public:
    ~UseCaseRegistrationWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {
        window_manager_ptr = wind_manager_ptr;
    }
    void setRegistrationWindow(ptr_to_iregistration_window reg_window_ptr) override {
        registration_window_ptr = reg_window_ptr;
    }
    void setRegistrationNetwork(ptr_to_iregistration_network reg_net_ptr) override {
        registration_network_ptr = reg_net_ptr;
    }
    ptr_to_iregistration_window getRegistrationWindow() override {
        return registration_window_ptr;
    }
    void regHandler(const std::string& email,
                    const std::string& login,
                    const std::string& password,
                    const std::string& pass_confirm) override {
        std::cout << "In reg hand" <<std::endl;
        if (login.empty() || password.empty() || email.empty() ||
        pass_confirm.empty()) {
                    //go back to auth window error
                    std::cout<<"bad" <<std::endl;
                    Error error_message;
                    error_message.type = "EmptyInput";
                    error_message.message = "Email, login, password or confirmation password was not inputed!";
                    //registration_window_ptr->createErrorMessage(error_message);
                    //registration_window_ptr->showErrorMessage();
                    sendError(error_message);
                    return;
        } else if (password != pass_confirm) {
                    std::cout<<"bad pass" <<std::endl;
                    Error error_message;
                    error_message.type = "NotEqualPasswords!";
                    error_message.message = "Password and confirmed password are different!";
                    //registration_window_ptr->createErrorMessage(error_message);
                    //registration_window_ptr->showErrorMessage();
                    sendError(error_message);
                    return;
        } else {
                RegInput input;
                input.email= email;
                input.login = login;
                input.password = password;
                std::cout << input.email <<' ' <<input.login <<' ' <<input.password <<std::endl;
                std::cout <<"here" <<std::endl;
                registration_network_ptr->getRegistration(input);
        }

    }
    void passToMain() override {}
    void sendError(const Error& error_message) override {
        registration_window_ptr->createErrorMessage(error_message);
        registration_window_ptr->showErrorMessage();
    }
    void setUser(const std::string& user) override {}
    std::string getUrl() override {}
    void setUrl(const std::string& url) override {}
private:
    ptr_to_iregistration_network registration_network_ptr;
    ptr_to_iregistration_window registration_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
};

#endif // USECASE_REGISTRATIONWINDOW_H
