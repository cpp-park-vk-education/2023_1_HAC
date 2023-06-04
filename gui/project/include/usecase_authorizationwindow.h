#ifndef USECASE_AUTHORIZATIONWINDOW_H
#define USECASE_AUTHORIZATIONWINDOW_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/usecase_authorizationwindow_interface.h"

class UseCaseAuthorizationWindow : public IHandlerAuthorizationWindow {
public:
    ~UseCaseAuthorizationWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override;
    void setAuthorizationWindow(
            ptr_to_iauthorization_window auth_window_ptr) override;
    void setAuthorizationNetwork(
            ptr_to_iauthorization_network auth_network_ptr) override;
    ptr_to_iauthorization_window getAuthorizationWindow() override;
    void authHandler(const std::string& login,
                     const std::string& password) override;
    void passToMain(const std::string& user) override;
    void sendError(const Error& error_message) override;
    void setUser(const std::string& user) override;
    void checkCookie() override;
    void checkedCookie(const std::string& user, const std::string& status)
    override;

    void openRegistrationWindow() override;
private:
    ptr_to_iauthorization_network authorization_network_ptr;
    ptr_to_iauthorization_window authorization_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
    std::string user_;
};

#endif // USECASE_AUTHORIZATIONWINDOW_H
