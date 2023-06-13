#ifndef USECASE_MAINWINDOW_H
#define USECASE_MAINWINDOW_H

#include <string>
#include <vector>
#include <iostream>

#include "../include/usecase_mainwindow_interface.h"

class UseCaseMainWindow : public IHandlerMainWindow {
public:
    ~UseCaseMainWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override;
    void setMainWindow(ptr_to_imain_window main_wind_ptr) override;
    void setMainNetwork(ptr_to_imain_network main_net_ptr) override;
    void drawPlotHandler(std::istream& network_output) override;
    void stockSelectHandler(const std::string& stock_name) override;
    void predictHandler(const std::string& stock_name, int wind_size) override;
    void sendError(const Error& error_message) override;
    void openUserSettings() override;

    void getActionsDataHandler() override;
    void setActionsDataHandler(std::istream& network_output) override;

    bool replaceSubstr(std::string& str, const std::string& from, const
    std::string& to);

private:
    ptr_to_imain_window main_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
    ptr_to_imain_network main_network_ptr;
    std::vector<double> y;
    std::vector<std::string> dates;
};

#endif // USECASE_MAINWINDOW_H
