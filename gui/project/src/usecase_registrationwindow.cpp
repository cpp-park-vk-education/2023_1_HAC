#include "../include/usecase_registrationwindow.h"

#include <iostream>

void UseCaseRegistrationWindow::setWindowManager(
        ptr_to_iwindow_manager wind_manager_ptr)  {
    window_manager_ptr = wind_manager_ptr;
}

void UseCaseRegistrationWindow::setRegistrationWindow(
        ptr_to_iregistration_window reg_window_ptr) {
    registration_window_ptr = reg_window_ptr;
}

void UseCaseRegistrationWindow::setRegistrationNetwork(
        ptr_to_iregistration_network reg_net_ptr) {
    registration_network_ptr = reg_net_ptr;
}

ptr_to_iregistration_window UseCaseRegistrationWindow::getRegistrationWindow() {
    return registration_window_ptr;
}

void UseCaseRegistrationWindow::regHandler(const std::string& email,
                const std::string& login,
                const std::string& password,
                const std::string& pass_confirm)  {
    if (password != pass_confirm) {
        Error error_message;
        error_message.type = "NotEqualPasswords!";
        error_message.message = "Password and confirmed password are different!";
        sendError(error_message);
        return;
    }

    if (login.empty() || password.empty()) {
        Error error_message;
        error_message.type = "EmptyInput";
        error_message.message = "Login or password password were not inputed!";
        sendError(error_message);
        return;
    }

    if (email.empty()) {
        Error error_message;
        error_message.type = "EmptyInput";
        error_message.message = "Email was not inputed!";
        sendError(error_message);
        return;
    }
    RegInput input;
    input.email= email;
    input.login = login;
    input.password = password;
    registration_network_ptr->getRegistration(input);

}

void UseCaseRegistrationWindow::passToMain(const std::string& user) {
    window_manager_ptr->setUser(user);
    window_manager_ptr->openMainWindow();
}

void UseCaseRegistrationWindow::sendError(const Error& error_message) {
    registration_window_ptr->createErrorMessage(error_message);
    registration_window_ptr->showErrorMessage();
}

void UseCaseRegistrationWindow::openAuthorizationWindow() {
    window_manager_ptr->openAuthorizationWindow();
}
