#ifndef USECASE_AUTHORIZATIONWINDOW_H
#define USECASE_AUTHORIZATIONWINDOW_H

#include <string>
#include "../include/usecase_authorizationwindow_interface.h"
#include <iostream>


class UseCaseAuthorizationWindow : public IHandlerAuthorizationWindow {
public:
    ~UseCaseAuthorizationWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {
        window_manager_ptr = wind_manager_ptr;
    }
    void setAuthorizationWindow(
            ptr_to_iauthorization_window auth_window_ptr) override {
        authorization_window_ptr = auth_window_ptr;
    }
    void setAuthorizationNetwork(ptr_to_iauthorization_network auth_network_ptr) override {
        authorization_network_ptr = auth_network_ptr;
    }
    ptr_to_iauthorization_window getAuthorizationWindow() override {
        return authorization_window_ptr;
    }
    void authHandler(const std::string& login, const std::string& password)
    override {
        std::cout << login<<" " << password <<std::endl;
        std::cout  << "1 "  <<" 2" <<std::endl;
        if (login.empty() || password.empty())  {
            //go back to auth window error
            std::cout<<"bad" <<std::endl;
            Error error_message;
            error_message.type = "EmptyInput";
            error_message.message = "Login or password was not inputed!";
            authorization_window_ptr->createErrorMessage(error_message);
            authorization_window_ptr->showErrorMessage();
            return;
        } else {
        AuthInput input;
        input.login = login;
        input.password = password;
        authorization_network_ptr->getAuthorization(input);
        }
    }
    void passToMain(const std::string& user) override {
        std::cout << "In pass to main" <<' ' << user <<std::endl;
        window_manager_ptr->setUser(user);
        std::cout << window_manager_ptr->getUser() <<std::endl;
        //go to mainwindow
        window_manager_ptr->openMainWindow();
    }
    void sendError(const Error& error_message) override {
        authorization_window_ptr->createErrorMessage(error_message);
        authorization_window_ptr->showErrorMessage();
    }
    void setUser(const std::string& user) override {}
    std::string getUrl() override {
        return url_;
    }
    void setUrl(const std::string& url) override {
        url_ = url;
    }
    void openRegistrationWindow() override {
        window_manager_ptr->openRegistrationWindow();
    }


private:
    ptr_to_iauthorization_network authorization_network_ptr;
    ptr_to_iauthorization_window authorization_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
    std::string url_;
};

#endif // USECASE_AUTHORIZATIONWINDOW_H
