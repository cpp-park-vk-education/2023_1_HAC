#include "utils.h"

#include "ionetwork_interface.h"
#include "settings/usecase_passwordsettingswindow.h"
#include "settings/passwordsettingswindow_interface.h"
#include "settings/usersettingswindow_network.h"

class MNetworkUserSettingsWindow : public INetworkUserSettingsWindow {
public:
    void setUserPasswordSettingsHandler(ptr_to_passwordsettings_handler
    set_passwordhandler_ptr) override {}
    void setUserEmailSettingsHandler(ptr_to_emailsettings_handler
    set_emailhandler_ptr) override {}
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

    void PostRequest(const std::string& url, const std::string& body, std::function<void(const Error& error_state)> callback) override {
        url_ = url;
    }

    void GetRequest(const std::string& url, std::istream& body, std::function<void(std::istream& network_output,
                                    const Error& error_state)> callback) override {}

    void setConfig(const std::string& host) override { }
    void setCookie(const std::string& cookie_data) override {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override{}

    std::string url_;
};

class SMPasswordSettingsWindow : public IPasswordSettingsWindow {
public:
    ~SMPasswordSettingsWindow() {}
    void setPasswordSettingsWindowHandler(ptr_to_passwordsettings_handler handler_user_set_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getOldPassword() override {return {};}
    std::string getNewPassword() override {return {};}
    std::string getRepeatPassword() override {return {};}
    Error error_message_;
};

class StubMPasswordSettingsWindow : public IPasswordSettingsWindow {
public:
    ~StubMPasswordSettingsWindow() {}
    void setPasswordSettingsWindowHandler(
            ptr_to_passwordsettings_handler handler_user_set_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getOldPassword() override {return "old";}
    std::string getNewPassword() override {return "new";}
    std::string getRepeatPassword() override {return "repeated";}
    Error error_message_;
};

class NewPassSettingsWindow : public IPasswordSettingsWindow {
public:
    ~NewPassSettingsWindow() {}
    void setPasswordSettingsWindowHandler(
            ptr_to_passwordsettings_handler handler_user_set_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getOldPassword() override {return "old";}
    std::string getNewPassword() override {return "";}
    std::string getRepeatPassword() override {return "";}
    Error error_message_;
};

class OldPasswordSMUSettingsWindow : public IPasswordSettingsWindow {
public:
    ~OldPasswordSMUSettingsWindow() {}
    void setPasswordSettingsWindowHandler(
            ptr_to_passwordsettings_handler handler_user_set_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getOldPassword() override {return "";}
    std::string getNewPassword() override {return "new";}
    std::string getRepeatPassword() override {return "new";}
    Error error_message_;
};

class MockUseCaseUserSettingsWindow : public IHandlerPasswordSettingsWindow {
public:
    ~MockUseCaseUserSettingsWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setPasswordSettingsWindowHandler(ptr_to_ipasswordsettings_window user_set_ptr) override {}
    void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr) override {}
    ptr_to_ipasswordsettings_window getPasswordSettingsWindow() override {return nullptr;}
    void sendError(const Error &error_message) override {
        error_type = error_message.type;
    }
    void passToMain() override {}
    void confirmHandler(const std::string& old_password,
                        const std::string& new_password,
                        const std::string&  repeat_password) override {}
    std::string getUser() override {return{};}
    void setUser(const std::string &user) override {}
    std::string error_type;
};

//check that getEditUserSettings is called from confirmHandler
TEST(PasswordSetQtLogicTest, TestUserSettingsHandler) {
    UseCasePasswordSettingsWindow handler_user_set;
    MNetworkUserSettingsWindow net_user_set;
    std::shared_ptr<MNetworkUserSettingsWindow> ptr_to_net =
            std::make_shared<MNetworkUserSettingsWindow>(net_user_set);
    handler_user_set.setUserSettingsNetwork(ptr_to_net);
    handler_user_set.setUser("user");
    handler_user_set.confirmHandler("old", "new", "new");
    std::string expected = "old";
    EXPECT_EQ(expected, ptr_to_net->confirm_params.old_password);
}

//check that PostRequest is called from getEditUserSettings
TEST(PasswordSetQtLogicTest, TestUserSettingsNet) {
    NetworkUserSettingsWindow net_user_set;
    MIONetworkInterface net;
    std::shared_ptr<MIONetworkInterface> ptr_to_net =
            std::make_shared<MIONetworkInterface>(net);
    net_user_set.setUserSettingsNetwork(ptr_to_net);
    net_user_set.setUrl("url");
    ConfirmEdit confirm_input;
    confirm_input.old_password = "old";
    confirm_input.new_password = "new";
    confirm_input.user_name= "user";
    net_user_set.getUserPasswordSettings(confirm_input);
    std::string expected = "url";
    EXPECT_EQ(expected, ptr_to_net->url_);
}

//check if old password or new passwords are not set (new password and old
// are equal (""))
TEST(PasswordSetQtLogicTest, TestUserSettingsOldIsNew) {
    UseCasePasswordSettingsWindow handler_user_set;
    SMPasswordSettingsWindow  user_set_window;
    std::shared_ptr<SMPasswordSettingsWindow> ptr_to_user_set_window =
            std::make_shared<SMPasswordSettingsWindow>(user_set_window);
    handler_user_set.setPasswordSettingsWindowHandler(ptr_to_user_set_window);
    handler_user_set.confirmHandler(handler_user_set
                                     .getPasswordSettingsWindow()->getOldPassword(),
                                    handler_user_set
                                     .getPasswordSettingsWindow()->getNewPassword(),
                                    handler_user_set
                                     .getPasswordSettingsWindow()->getRepeatPassword());
    std::string expected = "BadNewPassword";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

//check if repeated password or new passwords are not equal
TEST(PasswordSetQtLogicTest, TestUserSettingsNewNotConfirmed) {
    UseCasePasswordSettingsWindow handler_user_set;
    StubMPasswordSettingsWindow  user_set_window;
    std::shared_ptr<StubMPasswordSettingsWindow> ptr_to_user_set_window =
            std::make_shared<StubMPasswordSettingsWindow>(user_set_window);
    handler_user_set.setPasswordSettingsWindowHandler(ptr_to_user_set_window);
    handler_user_set.confirmHandler(handler_user_set
                                            .getPasswordSettingsWindow()->getOldPassword(),
                                    handler_user_set
                                            .getPasswordSettingsWindow()->getNewPassword(),
                                    handler_user_set
                                            .getPasswordSettingsWindow()->getRepeatPassword());
    std::string expected = "BadNewPassword";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

//check if new password is empty
TEST(PasswordSetQtLogicTest, TestUserSettingsNewIsEmpty) {
    UseCasePasswordSettingsWindow handler_user_set;
    NewPassSettingsWindow  user_set_window;
    std::shared_ptr<NewPassSettingsWindow> ptr_to_user_set_window =
            std::make_shared<NewPassSettingsWindow>(user_set_window);
    handler_user_set.setPasswordSettingsWindowHandler(ptr_to_user_set_window);
    handler_user_set.confirmHandler(handler_user_set
                                            .getPasswordSettingsWindow()->getOldPassword(),
                                    handler_user_set
                                            .getPasswordSettingsWindow()->getNewPassword(),
                                    handler_user_set
                                            .getPasswordSettingsWindow()->getRepeatPassword());
    std::string expected = "BadPassword";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

//check if old password is empty
TEST(PasswordSetQtLogicTest, TestUserSettingsOldIsEmpty) {
    UseCasePasswordSettingsWindow handler_user_set;
    OldPasswordSMUSettingsWindow  user_set_window;
    std::shared_ptr<OldPasswordSMUSettingsWindow> ptr_to_user_set_window =
            std::make_shared<OldPasswordSMUSettingsWindow>(user_set_window);
    handler_user_set.setPasswordSettingsWindowHandler(ptr_to_user_set_window);
    handler_user_set.confirmHandler(handler_user_set
                                            .getPasswordSettingsWindow()->getOldPassword(),
                                    handler_user_set
                                            .getPasswordSettingsWindow()->getNewPassword(),
                                    handler_user_set
                                            .getPasswordSettingsWindow()->getRepeatPassword());
    std::string expected = "BadPassword";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

//check if error is set
TEST(PasswordSetQtLogicTest, TestUserSettingsErrorRespose) {
    NetworkUserSettingsWindow net_user_set;
    MockUseCaseUserSettingsWindow handler_user_set;
    std::shared_ptr<MockUseCaseUserSettingsWindow> ptr_to_user_set_handler =
            std::make_shared<MockUseCaseUserSettingsWindow>(handler_user_set);
    net_user_set.setUserPasswordSettingsHandler(ptr_to_user_set_handler);
    Error error;
    error.type = "InvalidPassword";
    error.message = "Incorrect password was inputted";
    net_user_set.onGetUserPasswordSettingsResponse(error);
    std::string expected = "InvalidPassword";
    EXPECT_EQ(expected, ptr_to_user_set_handler->error_type);
}
