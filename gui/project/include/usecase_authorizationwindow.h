#ifndef USECASE_AUTHORIZATIONWINDOW_H
#define USECASE_AUTHORIZATIONWINDOW_H

#include <string>
#include "../include/usecase_authorizationwindow_interface.h"


class UseCaseAuthorizationWindow : public IHandlerAuthorizationWindow {
public:
    ~UseCaseAuthorizationWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setAuthorizationWindow(
            ptr_to_iauthorization_window auth_window_ptr) override {}
    void setAuthorizationNetwork(ptr_to_iauthorization_network auth_network_ptr) override {}
    ptr_to_iauthorization_window getAuthorizationWindow() override {return
    nullptr;}
    void authHandler(const std::string& login, const std::string& password)
    override {}
    void passToMain() override {}
    void sendError(const Error& error_message) override {}
    void setUser(const std::string user) override {};
    std::string getUrl() override {};
    void setUrl(const std::string url) override {};
private:
    ptr_to_iauthorization_network authorization_network_ptr;
    ptr_to_iauthorization_window authorization_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
};

#endif // USECASE_AUTHORIZATIONWINDOW_H