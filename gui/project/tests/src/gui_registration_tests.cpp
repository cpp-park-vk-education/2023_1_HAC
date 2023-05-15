#include "utils.h"

#include "registrationwindow_network.h"
#include "usecase_registrationwindow.h"
#include "ionetwork_interface.h"

class MNetworkRegistrationWindow : public INetworkRegistrationWindow {
public:
    void setRegistrationHandler(ptr_to_registration_handler reg_handler_ptr) override {}
    void setRegistrationNetwork(ptr_to_inetwork net_ptr) override {}
    void getRegistration(const RegInput& reg_params) override {}
    void onGetRegistrationResponse(const Error& error_state) override {}
    void setUser(const std::string user) override {}
    std::string getUrl() override {}
    void setUrl(const std::string url) override {}
    RegInput reg_params_;
};

class MIONetworkInterface : public IONetworkInterface {
public:
    void PostRequest(const std::string& url, const std::string& body,
                             std::function<void(
                                     const Error& error_state)> callback)
                                     override {
        url_ = url;
    }
    void GetRequest(const std::string& url, const MainData& body,
                            std::function<void(
                                    std::istream& network_output,
                                    const Error& error_state)> callback)
                                    override {}
    void setConfig(const std::string& host) override {}
    std::string url_;
};

class SMRegistrationWindow : public IRegistrationWindow {
public:
    ~SMRegistrationWindow() {}
    void setRegistrationWindowHandler(
            ptr_to_registration_handler reg_handler_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getLogin() override {
        return {};
    }
    std::string getPassword() override {
        return {};
    }
    std::string getEmail() override {
        return {};
    }
    Error error_message_;
};

class MockUseCaseRegistrationWindow : public IHandlerRegistrationWindow {
public:
    ~MockUseCaseRegistrationWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setRegistrationWindow(ptr_to_iregistration_window reg_window_ptr)
    override {}
    void setRegistrationNetwork(ptr_to_iregistration_network reg_net_ptr)
    override {}
    ptr_to_iregistration_window getRegistrationWindow() override {}
    void regHandler(const std::string &login,
                    const std::string &password) override {}
    void passToMain() override {}
    void sendError(const Error &error_message) override {
        error_type = error_message.type;
    }
    void setUser(const std::string &user) override {
        user_ = user;
    }
    std::string getUrl() override {}
    void setUrl(const std::string &url) override {}
    std::string error_type;
    std::string user_;
};

//check that getRegistration is called from regHandler
TEST(RegistrationQtLogicTest, TestRegHandler) {
    UseCaseRegistrationWindow handler_reg;
    MNetworkRegistrationWindow net_reg;
    std::shared_ptr<MNetworkRegistrationWindow> ptr_to_net =
            std::make_shared<MNetworkRegistrationWindow>(net_reg);
    handler_reg.setRegistrationNetwork(ptr_to_net);
    handler_reg.regHandler("User", "Password");
    std::string expected = "User";
    EXPECT_EQ(ptr_to_net->reg_params_.login, expected);
}

//check that PostRequest is called from getRegistration
TEST(RegistrationQtLogicTest, TestRegNet) {
    NetworkRegistrationWindow check_net_auth;
    MIONetworkInterface net;
    std::shared_ptr<MIONetworkInterface> ptr_to_net =
            std::make_shared<MIONetworkInterface>(net);
    check_net_auth.setRegistrationNetwork(ptr_to_net);
    check_net_auth.setUrl("url");
    RegInput auth_input;
    auth_input.login = "User";
    auth_input.password = "Password";
    check_net_auth.getRegistration(auth_input);
    std::string expected = "url";
    EXPECT_EQ(ptr_to_net->url_, expected);
}

//check if password and login are not set
TEST(RegistrationQtLogicTest, TestRegEmptyInput) {
    UseCaseRegistrationWindow handler_reg;
    SMRegistrationWindow reg_window;
    std::shared_ptr<SMRegistrationWindow> ptr_to_reg_window =
            std::make_shared<SMRegistrationWindow>(reg_window);
    handler_reg.setRegistrationWindow(ptr_to_reg_window);
    handler_reg.regHandler(handler_reg
                                     .getRegistrationWindow()->getLogin(),
                                     handler_reg
                                     .getRegistrationWindow()->getPassword());
    std::string expected = "EmptyInput";
    EXPECT_EQ(reg_window.error_message_.type, expected);
}

TEST(RegistrationQtLogicTest, TestRegSuccessRespose) {
    NetworkRegistrationWindow net_reg;
    MockUseCaseRegistrationWindow handler_reg;
    std::shared_ptr<MockUseCaseRegistrationWindow> ptr_to_auth_handler =
            std::make_shared<MockUseCaseRegistrationWindow>(handler_reg);
    net_reg.setRegistrationHandler(ptr_to_auth_handler);
    Error error;
    error.type = "0";
    error.message = "user";
    net_reg.onGetRegistrationResponse(error);
    std::string expected = "user";
    EXPECT_EQ(handler_reg.user_, expected);
}

TEST(RegistrationQtLogicTest, TestRegErrorRespose) {
    NetworkRegistrationWindow net_reg;
    MockUseCaseRegistrationWindow handler_reg;
    std::shared_ptr<MockUseCaseRegistrationWindow> ptr_to_auth_handler =
            std::make_shared<MockUseCaseRegistrationWindow>(handler_reg);
    net_reg.setRegistrationHandler(ptr_to_auth_handler);
    Error error;
    error.type = "InvalidPassword";
    error.message = "Incorrect password was inputted";
    net_reg.onGetRegistrationResponse(error);
    std::string expected = "InvalidPassword";
    EXPECT_EQ(handler_reg.error_type, expected);
}
