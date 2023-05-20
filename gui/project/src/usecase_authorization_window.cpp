#include "../include/usecase_authorizationwindow.h"

void UseCaseAuthorizationWindow::setWindowManager(
        ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}
void UseCaseAuthorizationWindow::setAuthorizationWindow(
        ptr_to_iauthorization_window auth_window_ptr) {
    authorization_window_ptr = auth_window_ptr;
}
void UseCaseAuthorizationWindow::setAuthorizationNetwork(
        ptr_to_iauthorization_network auth_network_ptr)  {
    authorization_network_ptr = auth_network_ptr;
}
ptr_to_iauthorization_window UseCaseAuthorizationWindow::getAuthorizationWindow() {
    return authorization_window_ptr;
}
void UseCaseAuthorizationWindow::authHandler(const std::string& login,
                                              const std::string& password) {
    std::cout << login<<" " << password <<std::endl;
    std::cout  << "1 "  <<" 2" <<std::endl;
    if (login.empty() || password.empty())  {
    //go back to auth window error
        std::cout<<"bad" <<std::endl;
        Error error_message;
        error_message.type = "EmptyInput";
        error_message.message = "Login or password was not inputed!";
        std::cout<<"before" <<std::endl;
        authorization_window_ptr->createErrorMessage(error_message);
        std::cout<<"after" <<std::endl;
        authorization_window_ptr->showErrorMessage();
        return;
    } else {
        AuthInput input;
        input.login = login;
        input.password = password;
        authorization_network_ptr->getAuthorization(input);
    }
}

void UseCaseAuthorizationWindow::passToMain(const std::string& user) {
    std::cout << "In pass to main" <<' ' << user <<std::endl;
    window_manager_ptr->setUser(user);
    std::cout << window_manager_ptr->getUser() <<std::endl;
    //go to mainwindow
    window_manager_ptr->openMainWindow();
}

void UseCaseAuthorizationWindow::sendError(const Error& error_message) {
    authorization_window_ptr->createErrorMessage(error_message);
    authorization_window_ptr->showErrorMessage();
}

void UseCaseAuthorizationWindow::openRegistrationWindow() {
    window_manager_ptr->openRegistrationWindow();
}