#ifndef USECASE_AUTHORIZATIONWINDOW_H
#define USECASE_AUTHORIZATIONWINDOW_H

#include <string>
#include "../include/usecase_authorizationwindow_interface.h"
#include <iostream>
#include <fstream>
#include <sstream>


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
    void setUser(const std::string& user) override {
        user_ = user;
    }
    std::string getUser() override {
        return user_;
    }
    /*void setUrl(const std::string& url) override {
        url_ = url;
    }*/
    void check_cookie() override {
        std::string line;
        std::string s;

        std::ifstream in;
        std::vector<std::string> lines;
        std::stringstream ss;

        in.open("../../project/auth_parm.txt");
        if (in.is_open()) {
            /*while (std::getline(in, line)) {
                    std::cout << line << std::endl;
            }*/
            std::getline(in, line);
            std::cout << line << std::endl;
            if (line.empty()) {
                std::cout <<"empty" <<std::endl;
            } else {
                ss << line;
                while(getline(ss, s, '\t')) {
                    lines.push_back(s);
                }
                std::cout << lines[0] << "+++" << lines[1] <<std::endl;
            }

            std::cout <<"open" <<std::endl;
            in.close();
            if (lines.size() == 2) {
                authorization_network_ptr->setCookie(lines[0], lines[1]);
            }
        } else {
            std::cout <<"not open" <<std::endl;
        }

    }
    void checked_cookie(const std::string& user, const std::string& status) override;

    void openRegistrationWindow() override;
private:
    ptr_to_iauthorization_network authorization_network_ptr;
    ptr_to_iauthorization_window authorization_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
    //std::string url_;
    std::string user_;
};

#endif // USECASE_AUTHORIZATIONWINDOW_H
