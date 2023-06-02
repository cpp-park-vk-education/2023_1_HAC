#include "utils.h"

#include "ionetwork_interface.h"
#include "usecase_emailsettingswindow.h"
#include "emailsettingswindow_interface.h"
#include "usersettingswindow_network.h"

class MNetworkUserSettingsWindow : public INetworkUserSettingsWindow {
public:
    void setUserPasswordSettingsHandler(ptr_to_passwordsettings_handler set_passwordhandler_ptr) override;
    void setUserEmailSettingsHandler(ptr_to_emailsettings_handler set_emailhandler_ptr) override;
    void setUserSettingsNetwork(ptr_to_inetwork net_ptr) override {}
    void getUserPasswordSettings(const ConfirmEdit& confirm_passwords) override {
        confirm_params = confirm_passwords;
    }

    void getUserEmailSettings(const ConfirmEdit& confirm_email) override {
        confirm_params = confirm_email;
    }
    void getUserExit(const std::string& user) override {}
    void onGetUserPasswordSettingsResponse(const Error& error_state) override {}
    void onGetUserEmailSettingsResponse(const Error& error_state) override {}
    void setUrl(const std::string& url) override {}
    ConfirmEdit confirm_params;
};

class MIONetworkInterface: public IONetworkInterface {
public:
    void PostRequest(const std::string& url, const std::string& body, std::function<void(const Error& error_state)> callback) {
        url_ = url;
    }

        void GetRequest(const std::string& url, std::istream& body, std::function<void(std::istream& network_output,
                                    const Error& error_state)> callback) override {}

    virtual void setConfig(const std::string& host) { }
    void setCookie(const std::string& cookie_data) override;
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override;

    std::string url_;
};

class SMEmailSettingsWindow : public IEmailSettingsWindow {
public:
    ~SMEmailSettingsWindow() {}
    void setEmailSettingsWindowHandler(ptr_to_emailsettings_handler set_handler_ptr) override {}

    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }

    std::string getNewEmail() override;
    std::string getPassword() override;

    Error error_message_;
};

class StubMEmailSettingsWindow : public IEmailSettingsWindow {
public:
    ~StubMEmailSettingsWindow() {}
    void setEmailSettingsWindowHandler(ptr_to_emailsettings_handler set_handler_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }

    std::string getNewEmail() override {return "email";}
    std::string getPassword() override {return "pass";}
    Error error_message_;
};

class NewEmailSMUserSettingsWindow : public IEmailSettingsWindow {
public:
    ~NewEmailSMUserSettingsWindow() {}
    void setEmailSettingsWindowHandler(ptr_to_emailsettings_handler set_handler_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }

    std::string getNewEmail() override {return "email";}
    std::string getPassword() override {return "";}
    Error error_message_;
};

class MockUseCaseUserSettingsWindow : public IHandlerEmailSettingsWindow {
public:
    ~MockUseCaseUserSettingsWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setEmailSettingsWindow(ptr_to_iemailsettings_window settings_wind_ptr) override {}
    void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) override {}
    ptr_to_iemailsettings_window getEmailSettingsWindow() override {}
    void sendError(const Error &error_message) override {
        error_type = error_message.type;
    }

    void passToMain() override {}
    void confirmHandler(const std::string& new_email, const std::string& password) override {}
    std::string getUser() override {}
    void setUser(const std::string &user) override {}
    std::string error_type;
};

//check that getEditUserSettings is called from confirmHandler
TEST(EmailSetQtLogicTest, TestEmailSettingsHandler) {
    UseCaseEmailSettingsWindow handler_user_set;
    MNetworkUserSettingsWindow net_user_set;
    std::shared_ptr<MNetworkUserSettingsWindow> ptr_to_net =
            std::make_shared<MNetworkUserSettingsWindow>(net_user_set);

    handler_user_set.setUserSettingsNetwork(ptr_to_net);
    handler_user_set.setUser("user");
    handler_user_set.confirmHandler("email", "pass");
    std::string expected = "email";
    EXPECT_EQ(expected, ptr_to_net->confirm_params.new_email);
}

//check that PostRequest is called from getEditUserSettings
TEST(EmailSetQtLogicTest, TestUserSettingsNet) {
    NetworkUserSettingsWindow net_user_set;
    MIONetworkInterface net;
    std::shared_ptr<MIONetworkInterface> ptr_to_net = std::make_shared<MIONetworkInterface>(net);
    net_user_set.setUserSettingsNetwork(ptr_to_net);
    net_user_set.setUrl("url");
    ConfirmEdit confirm_input;
    confirm_input.new_email = "old";
    confirm_input.old_password = "new";
    confirm_input.user_name= "user";
    net_user_set.getUserEmailSettings(confirm_input);
    std::string expected = "url";
    EXPECT_EQ(expected, ptr_to_net->url_);
}

// Empty email or password
TEST(EmailSetQtLogicTest, TestUserSettingsOldIsNew) {
    UseCaseEmailSettingsWindow handler_user_set;
    NewEmailSMUserSettingsWindow  user_set_window;
    std::shared_ptr<NewEmailSMUserSettingsWindow> ptr_to_user_set_window =
            std::make_shared<NewEmailSMUserSettingsWindow>(user_set_window);

    handler_user_set.setEmailSettingsWindow(ptr_to_user_set_window);
    handler_user_set.confirmHandler(handler_user_set
                                     .getEmailSettingsWindow()->getNewEmail(),
                                    handler_user_set
                                     .getEmailSettingsWindow()->getPassword());

    std::string expected = "BadEmail";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

// Error from server
TEST(EmailSetQtLogicTest, TestUserSettingsErrorRespose) {
    NetworkUserSettingsWindow net_user_set;
    MockUseCaseUserSettingsWindow handler_user_set;
    std::shared_ptr<MockUseCaseUserSettingsWindow> ptr_to_user_set_handler =
            std::make_shared<MockUseCaseUserSettingsWindow>(handler_user_set);
    net_user_set.setUserEmailSettingsHandler(ptr_to_user_set_handler);
    Error error;
    error.type = "InvalidData";
    error.message = "Incorrect email was inputted";
    net_user_set.onGetUserEmailSettingsResponse(error);
    std::string expected = "InvalidData";
    EXPECT_EQ(expected, ptr_to_user_set_handler->error_type);
}
