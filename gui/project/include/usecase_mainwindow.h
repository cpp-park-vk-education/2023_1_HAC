#ifndef USECASE_MAINWINDOW_H
#define USECASE_MAINWINDOW_H

#include <string>
#include <vector>
#include <iostream>
#include "../include/usecase_mainwindow_interface.h"

class UseCaseMainWindow : public IHandlerMainWindow {
public:
    ~UseCaseMainWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {
        window_manager_ptr = wind_manager_ptr;
    };
    void setMainWindow(ptr_to_imain_window main_wind_ptr) override {
        main_window_ptr = main_wind_ptr;
    };
    void setMainNetwork(ptr_to_imain_network main_net_ptr) override {
        main_network_ptr = main_net_ptr;
    };
    void drawPlotHandler(std::istream& network_output) override {
        //распарсить
        std::vector<double> new_y;
        std::string s;
        std::getline(network_output, s, '\n');
        if (s == "predict") {
            //merge two vectors
            new_y.insert(new_y.end(), y.begin(), y.end());
        } else {
            //after parsed new_y
            y = new_y;
        }
        main_window_ptr->createPlot(new_y);
        main_window_ptr->drawPlot();
        return;
    };
    void stockSelectHandler(const std::string& stock_name) override {
        std::cout << "In stock selected: " <<stock_name<< std::endl;
        //stock_name_ = stock_name;
        MainData stock_data;
        stock_data.operation_title = "plot";
        stock_data.stock_name = stock_name;
        stock_data.lag = 7 * 24;
        stock_data.window_size = 0;
        main_network_ptr->getPlotData(stock_data);
    };
    void predictHandler(const std::string& stock_name) override {
        std::cout << "In predict Handler: " <<stock_name<< std::endl;
        //stock_name_ = stock_name;
        stockSelectHandler(stock_name); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        MainData stock_data;
        stock_data.operation_title = "predict";
        stock_data.stock_name = stock_name;
        stock_data.lag = 8;
        stock_data.window_size = 0;
        main_network_ptr->getPredictData(stock_data);
    };
    void sendError(const Error& error_message) override {
        main_window_ptr->createErrorMessage(error_message);
        main_window_ptr->showErrorMessage();
    };
    std::string getUrl() override {};
    void setUrl(const std::string& url) override {};
    void openUserSettings() override {
        std::cout <<"handler" <<std::endl;
        window_manager_ptr->openUserSettingsWindow();
    }
private:
    ptr_to_imain_window main_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
    ptr_to_imain_network main_network_ptr;
    std::vector<double> y;

    //std::string stock_name_;
};

#endif // USECASE_MAINWINDOW_H
