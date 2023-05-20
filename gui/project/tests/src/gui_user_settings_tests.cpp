#include "utils.h"

#include "ionetwork_interface.h"
#include "usecase_usersettingswindow.h"
#include "usersettingswindow_interface.h"
#include "usersettingswindow_network.h"

class MNetworkUserSettingsWindow : public INetworkUserSettingsWindow {
public:
    void
    setUserSettingsHandler(ptr_to_settings_handler set_handler_ptr) override {}
    void setUserSettingsNetwork(ptr_to_inetwork net_ptr) override {}
    void getUserSettings(const ConfirmEdit &confirm_passwords) override {
        confirm_params = confirm_passwords;
    }
    void onGetUserSettingsResponse(const Error &error_state) override {}
    void setUrl(const std::string& url) override {}
    ConfirmEdit confirm_params;
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
                            std::istream& network_output,
                            const Error& error_state)> callback) {}
    virtual void setConfig(const std::string& host) { }
    std::string url_;
};

class SMUserSettingsWindow : public IUserSettingsWindow {
public:
    ~SMUserSettingsWindow() {}
    void setUserSettingsWindowHandler(
            ptr_to_settings_handler handler_user_set_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getOldPassword() override {return {};}
    std::string getNewPassword() override {return {};}
    std::string getRepeatPassword() override {return {};}
    Error error_message_;
};

class StubMUserSettingsWindow : public IUserSettingsWindow {
public:
    ~StubMUserSettingsWindow() {}
    void setUserSettingsWindowHandler(
            ptr_to_settings_handler handler_user_set_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getOldPassword() override {return "old";}
    std::string getNewPassword() override {return "new";}
    std::string getRepeatPassword() override {return "repeated";}
    Error error_message_;
};

class NewPasSMUserSettingsWindow : public IUserSettingsWindow {
public:
    ~NewPasSMUserSettingsWindow() {}
    void setUserSettingsWindowHandler(
            ptr_to_settings_handler handler_user_set_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getOldPassword() override {return "old";}
    std::string getNewPassword() override {return "";}
    std::string getRepeatPassword() override {return "";}
    Error error_message_;
};

class OldPasSMUserSettingsWindow : public IUserSettingsWindow {
public:
    ~OldPasSMUserSettingsWindow() {}
    void setUserSettingsWindowHandler(
            ptr_to_settings_handler handler_user_set_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {
        error_message_ = error_message;
    }
    std::string getOldPassword() override {return "";}
    std::string getNewPassword() override {return "new";}
    std::string getRepeatPassword() override {return "new";}
    Error error_message_;
};

class MockUseCaseUserSettingsWindow : public IHandlerUserSettingsWindow {
public:
    ~MockUseCaseUserSettingsWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setUserSettingsWindow(
            ptr_to_isettings_window user_set_ptr) override {}
    void setUserSettingsNetwork(ptr_to_isettings_network settings_net_ptr)
    override {}
    ptr_to_isettings_window getUserSettingsWindow() override {}
    void sendError(const Error &error_message) override {
        error_type = error_message.type;
    }
    void passToMain() override {}
    void ConfirmHandler(const std::string& old_password,
                        const std::string& new_password,
                        const std::string&  repeat_password) override {}
    std::string getUser() override {}
    void setUser(const std::string &user) override {}
    std::string error_type;
};

//check that getEditUserSettings is called from ConfirmHandler
TEST(UserSetQtLogicTest, TestUserSettingsHandler) {
    UseCaseUserSettingsWindow handler_user_set;
    MNetworkUserSettingsWindow net_user_set;
    std::shared_ptr<MNetworkUserSettingsWindow> ptr_to_net =
            std::make_shared<MNetworkUserSettingsWindow>(net_user_set);
    handler_user_set.setUserSettingsNetwork(ptr_to_net);
    handler_user_set.setUser("user");
    handler_user_set.ConfirmHandler("old", "new", "new");
    std::string expected = "old";
    EXPECT_EQ(expected, ptr_to_net->confirm_params.old_password);
}

//check that PostRequest is called from getEditUserSettings
TEST(UserSetQtLogicTest, TestUserSettingsNet) {
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
    net_user_set.getUserSettings(confirm_input);
    std::string expected = "url";
    EXPECT_EQ(expected, ptr_to_net->url_);
}

//check if old password or new passwords are not set (new password and old
// are equal (""))
TEST(UserSetQtLogicTest, TestUserSettingsOldIsNew) {
    UseCaseUserSettingsWindow handler_user_set;
    SMUserSettingsWindow  user_set_window;
    std::shared_ptr<SMUserSettingsWindow> ptr_to_user_set_window =
            std::make_shared<SMUserSettingsWindow>(user_set_window);
    handler_user_set.setUserSettingsWindow(ptr_to_user_set_window);
    handler_user_set.ConfirmHandler(handler_user_set
                                     .getUserSettingsWindow()->getOldPassword(),
                                    handler_user_set
                                     .getUserSettingsWindow()->getNewPassword(),
                                    handler_user_set
                                     .getUserSettingsWindow()->getRepeatPassword());
    std::string expected = "BadNewPassword";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

//check if repeated password or new passwords are not equal
TEST(UserSetQtLogicTest, TestUserSettingsNewNotConfirmed) {
    UseCaseUserSettingsWindow handler_user_set;
    StubMUserSettingsWindow  user_set_window;
    std::shared_ptr<StubMUserSettingsWindow> ptr_to_user_set_window =
            std::make_shared<StubMUserSettingsWindow>(user_set_window);
    handler_user_set.setUserSettingsWindow(ptr_to_user_set_window);
    handler_user_set.ConfirmHandler(handler_user_set
                                            .getUserSettingsWindow()->getOldPassword(),
                                    handler_user_set
                                            .getUserSettingsWindow()->getNewPassword(),
                                    handler_user_set
                                            .getUserSettingsWindow()->getRepeatPassword());
    std::string expected = "BadNewPassword";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

//check if new password is empty
TEST(UserSetQtLogicTest, TestUserSettingsNewIsEmpty) {
    UseCaseUserSettingsWindow handler_user_set;
    NewPasSMUserSettingsWindow  user_set_window;
    std::shared_ptr<NewPasSMUserSettingsWindow> ptr_to_user_set_window =
            std::make_shared<NewPasSMUserSettingsWindow>(user_set_window);
    handler_user_set.setUserSettingsWindow(ptr_to_user_set_window);
    handler_user_set.ConfirmHandler(handler_user_set
                                            .getUserSettingsWindow()->getOldPassword(),
                                    handler_user_set
                                            .getUserSettingsWindow()->getNewPassword(),
                                    handler_user_set
                                            .getUserSettingsWindow()->getRepeatPassword());
    std::string expected = "BadNewPassword";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

//check if old password is empty
TEST(UserSetQtLogicTest, TestUserSettingsOldIsEmpty) {
    UseCaseUserSettingsWindow handler_user_set;
    OldPasSMUserSettingsWindow  user_set_window;
    std::shared_ptr<OldPasSMUserSettingsWindow> ptr_to_user_set_window =
            std::make_shared<OldPasSMUserSettingsWindow>(user_set_window);
    handler_user_set.setUserSettingsWindow(ptr_to_user_set_window);
    handler_user_set.ConfirmHandler(handler_user_set
                                            .getUserSettingsWindow()->getOldPassword(),
                                    handler_user_set
                                            .getUserSettingsWindow()->getNewPassword(),
                                    handler_user_set
                                            .getUserSettingsWindow()->getRepeatPassword());
    std::string expected = "BadOldPassword";
    EXPECT_EQ(expected, ptr_to_user_set_window->error_message_.type);
}

TEST(UserSetQtLogicTest, TestUserSettingsErrorRespose) {
    NetworkUserSettingsWindow net_user_set;
    MockUseCaseUserSettingsWindow handler_user_set;
    std::shared_ptr<MockUseCaseUserSettingsWindow> ptr_to_user_set_handler =
            std::make_shared<MockUseCaseUserSettingsWindow>(handler_user_set);
    net_user_set.setUserSettingsHandler(ptr_to_user_set_handler);
    Error error;
    error.type = "InvalidPassword";
    error.message = "Incorrect password was inputted";
    net_user_set.onGetUserSettingsResponse(error);
    std::string expected = "InvalidPassword";
    EXPECT_EQ(expected, ptr_to_user_set_handler->error_type);
}
