#ifndef USECASE_MAINWINDOW_H
#define USECASE_MAINWINDOW_H

#include <QWidget>
//#include <memory>
#include <string>
//#include "../include/mainwindow_interface.h"
#include "../include/usecase_mainwindow_interface.h"

class UseCaseMainWindow : public IHandlerMainWindow {
public:
    ~UseCaseMainWindow() {};
    void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) override {};
    void setMainWindow(ptr_to_imain_window main_wind_ptr) override {};
    void setMainNetwork(ptr_to_imain_network main_net_ptr) override {};
    void drawPlotHandler(const std::istream& network_output) override {};
    void stockSelectHandler(const std::string& stock_name) override {};
    void predictHandler(const std::string& stock_name) override {};
    void sendError(const Error& error_message) override {};
    std::string getUrl() override {};
    void setUrl(const std::string url) override {};
private:
    ptr_to_imain_window main_window_ptr;
    ptr_to_iwindow_manager window_manager_ptr;
    ptr_to_imain_network main_network_ptr;
};

#endif // USECASE_MAINWINDOW_H
