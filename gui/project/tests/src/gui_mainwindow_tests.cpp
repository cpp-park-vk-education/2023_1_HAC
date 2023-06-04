#include "utils.h"

#include <sstream>

#include "ionetwork_interface.h"
#include "usecase_mainwindow.h"
#include "mainwindow_network.h"

class MNetworkMainWindow : public INetworkMainWindow {
public:
    void setMainHandler(ptr_to_main_handler handler_main_ptr) override {}
    void setMainNetwork(ptr_to_inetwork net_ptr) override {}
    void getPlotData(const MainData& plot_data) override {
        stock_ = plot_data.stock_name;
    }
    void getActionsData() override {}
    void onGetActionsData(std::istream& network_output,
                          const Error& error_state) override {}
    void getPredictData(const MainData& plot_data) override {}
    void onGetPredictData(std::istream& network_output,
                          const Error& error_state) override {}
    void onGetPlotData(std::istream& network_output,
                       const Error& error_state) override {}
    void setUrl(const std::string& url) override {}
    std::string stock_;
};

class MockNetworkMainWindow : public INetworkMainWindow {
public:
    void setMainHandler(ptr_to_main_handler handler_main_ptr) override {}
    void setMainNetwork(ptr_to_inetwork net_ptr) override {}
    void getPlotData(const MainData& plot_data) override {}
    void getPredictData(const MainData& plot_data) override {
        stock_ = plot_data.stock_name;
    }
    void getActionsData() override {}
    void onGetActionsData(std::istream& network_output,
                          const Error& error_state) override {}
    void onGetPredictData(std::istream& network_output,
                          const Error& error_state) override {}
    void onGetPlotData(std::istream& network_output,
                       const Error& error_state) override {}
    void setUrl(const std::string& url) override {}
    std::string stock_;
};

class MIONetwork : public IONetworkInterface {
public:
    void PostRequest(const std::string &url, const std::string &body,
                     std::function<void(const Error &error_state)>
                     callback) override {}
    void GetRequest(const std::string &url, std::istream& body,
                    std::function<void(std::istream &network_output,
                                       const Error &error_state)> callback)
    override {
        url_ = url;
    }
    void setConfig(const std::string &host) override {}
    void setCookie(const std::string& cookie_data) override {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    std::string url_;
};

class MockIONetwork : public IONetworkInterface {
public:
    void PostRequest(const std::string &url, const std::string &body,
                     std::function<void(const Error &error_state)>
                     callback) override {}
    void GetRequest(const std::string &url, std::istream& body,
                    std::function<void(std::istream &network_output,
                                       const Error &error_state)> callback)
    override {
        url_ = url;
    }
    void setConfig(const std::string &host) override {}
    void setCookie(const std::string& cookie_data) override {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    std::string url_;
};

class MUseCaseMainWindow : public IHandlerMainWindow {
public:
    ~MUseCaseMainWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setMainWindow(ptr_to_imain_window main_wind_ptr) override {}
    void setMainNetwork(ptr_to_imain_network main_net_ptr) override {}
    void drawPlotHandler(std::istream &network_output) override {
        std::stringstream output;
        /*while (network_output) {
            char c;
            network_output >> c;
            output << c;
        }*/
        std::string s;
        while (std::getline(network_output, s)) {
            output << s;
        }
        out = output.str();
    }
    void stockSelectHandler(const std::string &stock_name) override {}
    void predictHandler(const std::string &stock_name, int wind_size) override {}
    void getActionsDataHandler() override {}
    void setActionsDataHandler(std::istream& network_output) override {}
    void sendError(const Error &error_message) override {}
    void openUserSettings() override {}
    std::string out;
};

class MockUseCaseMainWindow : public IHandlerMainWindow {
public:
    ~MockUseCaseMainWindow() {}
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {}
    void setMainWindow(ptr_to_imain_window main_wind_ptr) override {}
    void setMainNetwork(ptr_to_imain_network main_net_ptr) override {}
    void drawPlotHandler(std::istream &network_output) override {}
    void stockSelectHandler(const std::string &stock_name) override {}
    void predictHandler(const std::string &stock_name, int wind_size) override {}
    void getActionsDataHandler() override {}
    void setActionsDataHandler(std::istream& network_output) override {}
    void sendError(const Error &error_message) override {
        error_type = error_message.type;
    }
    void openUserSettings() override {}
    std::string error_type;
};

//check that getPlotData is called from stockSelectedHandler
TEST(MainQtLogicTest, TestMainHandler) {
    UseCaseMainWindow check_handler_auth;
    MNetworkMainWindow net_main;
    std::shared_ptr<MNetworkMainWindow> ptr_to_net =
            std::make_shared<MNetworkMainWindow>(net_main);
    check_handler_auth.setMainNetwork(ptr_to_net);
    std::string input = "stock";
    check_handler_auth.stockSelectHandler(input);
    std::string expected = "stock";
    EXPECT_EQ(expected, ptr_to_net->stock_);
}

//check that getPredictData is called from predictHandler
TEST(MainQtLogicTest, TestMainPredictHandler) {
    UseCaseMainWindow check_handler_auth;
    MockNetworkMainWindow net_main;
    std::shared_ptr<MockNetworkMainWindow> ptr_to_net =
            std::make_shared<MockNetworkMainWindow>(net_main);
    check_handler_auth.setMainNetwork(ptr_to_net);
    std::string input = "stock";
    check_handler_auth.predictHandler(input, 8);
    std::string expected = "stock";
    EXPECT_EQ(expected, ptr_to_net->stock_);
}

//check that GetRequest is called from getPlotData
TEST(MainQtLogicTest, TestMainNet) {
    NetworkMainWindow check_net_main;
    MIONetwork net;
    std::shared_ptr<MIONetwork> ptr_to_net =
            std::make_shared<MIONetwork>(net);
    check_net_main.setMainNetwork(ptr_to_net);
    check_net_main.setUrl("url");
    MainData main_input;
    main_input.operation_title = "plain";
    main_input.stock_name = "stock";
    check_net_main.getPlotData(main_input);
    std::string expected = "url";
    EXPECT_EQ(expected, ptr_to_net->url_);
}

//check that GetRequest is called from getPredictData
TEST(MainQtLogicTest, TestMainPredictNet) {
    NetworkMainWindow check_net_main;
    MockIONetwork net;
    std::shared_ptr<MockIONetwork> ptr_to_net =
            std::make_shared<MockIONetwork>(net);
    check_net_main.setMainNetwork(ptr_to_net);
    check_net_main.setUrl("url");
    MainData main_input;
    main_input.operation_title = "predict";
    main_input.stock_name = "stock";
    check_net_main.getPredictData(main_input);
    std::string expected = "url";
    EXPECT_EQ(expected, ptr_to_net->url_);
}

TEST(MainQtLogicTest, TestMainSuccessRespose) {
    NetworkMainWindow net_main;
    MUseCaseMainWindow handler_main;
    std::shared_ptr<MUseCaseMainWindow> ptr_to_main_handler =
            std::make_shared<MUseCaseMainWindow>(handler_main);
    net_main.setMainHandler(ptr_to_main_handler);
    Error error;
    error.type = "0";
    std::stringstream output;
    output << "123";
    net_main.onGetPlotData(output,error);
    std::string expected = "plot123";
    EXPECT_EQ(expected, ptr_to_main_handler->out);
}

TEST(MainQtLogicTest, TestMainPredictSuccessRespose) {
    NetworkMainWindow net_main;
    MUseCaseMainWindow handler_main;
    std::shared_ptr<MUseCaseMainWindow> ptr_to_main_handler =
            std::make_shared<MUseCaseMainWindow>(handler_main);
    net_main.setMainHandler(ptr_to_main_handler);
    Error error;
    error.type = "0";
    std::stringstream output;
    output << "123";
    net_main.onGetPredictData(output,error);
    std::string expected = "predict123";
    EXPECT_EQ(expected, ptr_to_main_handler->out);
}

TEST(MainQtLogicTest, TestMainErrorRespose) {
    NetworkMainWindow net_main;
    MockUseCaseMainWindow handler_main;
    std::shared_ptr<MockUseCaseMainWindow> ptr_to_main_handler =
            std::make_shared<MockUseCaseMainWindow>(handler_main);
    net_main.setMainHandler(ptr_to_main_handler);
    Error error;
    error.type = "ErrorDataType";
    error.message = "Incorrect data type";
    std::stringstream output;
    output << "adf1";
    net_main.onGetPlotData(output,error);
    std::string expected = "ErrorDataType";
    EXPECT_EQ(expected, ptr_to_main_handler->error_type);
}

TEST(MainQtLogicTest, TestMainPredictErrorRespose) {
    NetworkMainWindow net_main;
    MockUseCaseMainWindow handler_main;
    std::shared_ptr<MockUseCaseMainWindow> ptr_to_main_handler =
            std::make_shared<MockUseCaseMainWindow>(handler_main);
    net_main.setMainHandler(ptr_to_main_handler);
    Error error;
    error.type = "ErrorDataType";
    error.message = "Incorrect data type";
    std::stringstream output;
    output << "adf1";
    net_main.onGetPredictData(output,error);
    std::string expected = "ErrorDataType";
    EXPECT_EQ(expected, ptr_to_main_handler->error_type);
}
