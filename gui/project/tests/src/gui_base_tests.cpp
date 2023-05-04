#include "../include/utils.h"

#include "usecase_authorizationwindow_interface.h"
#include "authorizationwindow_interface.h"
#include "ionetwork_interface.h"
#include "authorizationwindow_network_interface.h"
#include "usecase_authorizationwindow.h"
#include "authorizationwindow_network.h"
//#include "project/include/usecase_authorizationwindow_interface.h"

/*
class HandlerAuthorizationWindow : public IHandlerAuthorizationWindow{
public:
    ~HandlerAuthorizationWindow() { }
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {  }
    void setAuthorizationWindow(
            ptr_to_iauthorization_window auth_window_ptr) override { }
    ptr_to_iauthorization_window getAuthorizationWindow() override {};
    void setAuthorizationNetwork(
            ptr_to_iauthorization_network auth_network_ptr) override {
        authorization_network_ptr = auth_network_ptr;
    }
    void authHandler(const std::string& login,
                             const std::string& password) override {
        login_ = login;
        password_ = password;
        auth_input.login = login;
        auth_input.password = password;
        authorization_network_ptr->getAuthorization(auth_input);
    }
    void passToMain() override { }
    void sendError(const Error& error_message) override {

    }

    ptr_to_iauthorization_network authorization_network_ptr;
    std::string login_;
    std::string password_;
    AuthInput auth_input;
};*/

class MNetworkAuthorizationWindow : public INetworkAuthorizationWindow {
public:
    void setAuthorizationHandler(
            ptr_to_authorization_handler auth_handler_ptr) override {}
    void setAuthorizationNetwork(ptr_to_inetwork net_ptr) override {}
    void getAuthorization(const AuthInput& auth_params) override {
        auth_params_.login = auth_params.login;
        //std::cout<<auth_params.login<<std::endl;
        //std::cout<<auth_params_.login<<std::endl;
        auth_params_.password = auth_params.password;
    }
    void onGetAuthorizationResponse(const Error& error_state) override {}
public:
    AuthInput auth_params_;
};


class SNetworkAuthorizationWindow : public INetworkAuthorizationWindow {
public:
    void setAuthorizationHandler(
            ptr_to_authorization_handler auth_handler_ptr) override {}
    void setAuthorizationNetwork(ptr_to_inetwork net_ptr) override {
        network_ptr = net_ptr;
    }
    void onGetAuthorizationResponse(const Error& error_state) override {}
    void getAuthorization(const AuthInput& auth_params) override {
        auth_params_.login = auth_params.login;
        //std::cout<<auth_params.login<<std::endl;
        //std::cout<<auth_params_.login<<std::endl;
        auth_params_.password = auth_params.password;
        std::string input_line =
                auth_params.login + '\t' + auth_params.password;
        network_ptr->PostRequest("url", input_line,
                                 [this](const Error& error_state)
                                 {onGetAuthorizationResponse
                                 (error_state);});
    }
public:
    AuthInput auth_params_;
    ptr_to_inetwork network_ptr;
};


class MIONetworkInterface: public IONetworkInterface {
public:
    void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(
                                     const Error& error_state)> callback) {
        url_ = url;
    }
    void GetRequest(const std::string& url, const MainData& body,
                            std::function<void(
                                    const std::istream& network_output,
                                    const Error& error_state)> callback) {}
    virtual void setConfig(const std::string& host) { }
    std::string url_;
};

class SMAuthorizationWindow : public IAuthorizationWindow {
public:
    ~SMAuthorizationWindow() { }
    void setAuthorizationWindowHandler(
            ptr_to_authorization_handler auth_handler_ptr) override { }
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override{ }
    std::string getLogin() override {
        return {};
    }
    std::string getPassword() override {
        return {};
    }
    Error error_message_;
};

//check that getAuthorization is called from authHandler
TEST(BaseQtLogicTest, TestAuthHandler) {
    UseCaseAuthorizationWindow check_handler_auth;
    MNetworkAuthorizationWindow net_auth;
    std::shared_ptr<MNetworkAuthorizationWindow> ptr_to_net =
            std::make_shared<MNetworkAuthorizationWindow>(net_auth);
    check_handler_auth.setAuthorizationNetwork(ptr_to_net);
    check_handler_auth.authHandler("User", "Password");
    std::string expected = "User";
    EXPECT_EQ(ptr_to_net->auth_params_.login, expected);
}

//check that PostRequest is called from getAuthorization
TEST(BaseQtLogicTest, TestAuthNet) {
    NetworkAuthorizationWindow check_net_auth;
    MIONetworkInterface net;
    std::shared_ptr<MIONetworkInterface> ptr_to_net =
            std::make_shared<MIONetworkInterface>(net);
    check_net_auth.setAuthorizationNetwork(ptr_to_net);
    AuthInput auth_input;
    auth_input.login = "User";
    auth_input.password = "Password";
    check_net_auth.getAuthorization(auth_input);
    std::string expected = "url";
    EXPECT_EQ(ptr_to_net->url_, expected);
}

//check if password and login are not set
/*
TEST(BaseQtLogicTest, TestAuthEmptyInput) {
    UseCaseAuthorizationWindow handler_auth;
    SMAuthorizationWindow auth_window;
    std::shared_ptr<SMAuthorizationWindow> ptr_to_auth_window =
            std::make_shared<SMAuthorizationWindow>(auth_window);
    handler_auth.setAuthorizationWindow(ptr_to_auth_window);
    handler_auth.authHandler(handler_auth
    .getAuthorizationWindow()->getLogin(), handler_auth
            .getAuthorizationWindow()->getPassword());
    std::string expected = "EmptyInput";
    EXPECT_EQ(auth_window.error_message_.type, expected);
}*/


TEST(TEST1, TEST2) {

    ASSERT_EQ(1,1);
    //EXPECT_THROW();
}
