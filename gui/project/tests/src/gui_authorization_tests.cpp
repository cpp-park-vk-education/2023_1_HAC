//#include "../include/utils.h"
#include "utils.h"

#include "usecase_authorizationwindow_interface.h"
#include "authorizationwindow_interface.h"
#include "ionetwork_interface.h"
#include "authorizationwindow_network_interface.h"
#include "usecase_authorizationwindow.h"
#include "authorizationwindow_network.h"

class MNetworkAuthorizationWindow : public INetworkAuthorizationWindow {
public:
    void setAuthorizationHandler(
            ptr_to_authorization_handler auth_handler_ptr) override {}
    void setAuthorizationNetwork(ptr_to_inetwork net_ptr) override {}
    void getAuthorization(const AuthInput& auth_params) override {
        auth_params_.login = auth_params.login;
        auth_params_.password = auth_params.password;
    }
    void onGetAuthorizationResponse(const Error& error_state) override {}
    void setCookie(const std::string& user,const std::string& cookie_line) override {};
    void onSetCookieResponse(const Error& error_state) override {};
    void setUrl(const std::string& url) override {}
public:
    AuthInput auth_params_;
};

class MIONetworkInterface: public IONetworkInterface {
public:
    void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(
                                     const Error& error_state)> callback) override {
        url_ = url;
    }
    void GetRequest(const std::string& url, std::istream&  body,
                            std::function<void(
                                    std::istream& network_output,
                                    const Error& error_state)> callback) override {}
    void setConfig(const std::string& host) override {}
    void setCookie(const std::string& cookie_data) override {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    std::string url_;
};

class SMAuthorizationWindow : public IAuthorizationWindow {
public:
    ~SMAuthorizationWindow() { }
    void setAuthorizationWindowHandler(
            ptr_to_authorization_handler auth_handler_ptr) override { }
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override{
        error_message_ = error_message;
    }
    std::string getLogin() override {
        error_message_.type = "";
        return {};
    }
    std::string getPassword() override {
        return error_message_.type;
    }
    Error error_message_;
};

class MockUseCaseAuthorizationWindow : public IHandlerAuthorizationWindow {
public:
    ~MockUseCaseAuthorizationWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setAuthorizationWindow(
            ptr_to_iauthorization_window auth_window_ptr) override {}
    void setAuthorizationNetwork(
            ptr_to_iauthorization_network auth_network_ptr) override {}
    ptr_to_iauthorization_window getAuthorizationWindow() override {return nullptr;}
    void authHandler(const std::string &login, const std::string &password)
    override {}
    void passToMain(const std::string& user) override {
        setUser(user);
    }
    void sendError(const Error &error_message) override {
        error_type = error_message.type;
    }
    void setUser(const std::string& user) override {
        user_ = user;
    }
    void openRegistrationWindow() override {}

    void checkCookie() override {}
    void checkedCookie(const std::string& user, const std::string& status) override {}
    std::string error_type;
    std::string user_;
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
    EXPECT_EQ(expected, ptr_to_net->auth_params_.login);
}

//check that PostRequest is called from getAuthorization
TEST(BaseQtLogicTest, TestAuthNet) {
    NetworkAuthorizationWindow check_net_auth;
    MIONetworkInterface net;
    std::shared_ptr<MIONetworkInterface> ptr_to_net =
            std::make_shared<MIONetworkInterface>(net);
    check_net_auth.setAuthorizationNetwork(ptr_to_net);
    check_net_auth.setUrl("url");
    AuthInput auth_input;
    auth_input.login = "User";
    auth_input.password = "Password";
    check_net_auth.getAuthorization(auth_input);
    std::string expected = "url";
    EXPECT_EQ(expected, ptr_to_net->url_);
}

//check if password and login are not set
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
    std::cout << handler_auth
            .getAuthorizationWindow()->getPassword()<<std::endl;
    EXPECT_EQ(expected, ptr_to_auth_window->error_message_.type);
}

TEST(BaseQtLogicTest, TestAuthSuccessRespose) {
    NetworkAuthorizationWindow net_auth;
    MockUseCaseAuthorizationWindow handler_auth;
    std::shared_ptr<MockUseCaseAuthorizationWindow> ptr_to_auth_handler =
            std::make_shared<MockUseCaseAuthorizationWindow>(handler_auth);
    net_auth.setAuthorizationHandler(ptr_to_auth_handler);
    Error error;
    error.type = "0";
    error.message = "user";
    net_auth.onGetAuthorizationResponse(error);
    std::string expected = "user";
    EXPECT_EQ(expected, ptr_to_auth_handler->user_);
}

TEST(BaseQtLogicTest, TestAuthErrorRespose) {
    NetworkAuthorizationWindow net_auth;
    MockUseCaseAuthorizationWindow handler_auth;
    std::shared_ptr<MockUseCaseAuthorizationWindow> ptr_to_auth_handler =
            std::make_shared<MockUseCaseAuthorizationWindow>(handler_auth);
    net_auth.setAuthorizationHandler(ptr_to_auth_handler);
    Error error;
    error.type = "InvalidPassword";
    error.message = "Incorrect password was inputted";
    net_auth.onGetAuthorizationResponse(error);
    std::string expected = "InvalidPassword";
    EXPECT_EQ(expected, ptr_to_auth_handler->error_type);
}

