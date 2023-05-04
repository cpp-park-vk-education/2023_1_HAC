#ifndef USECASE_MAINWINDOW_INTERFACE_H
#define USECASE_MAINWINDOW_INTERFACE_H

#include <memory>
#include "../include/mainwindow_interface.h"
#include "../include/mainwindow_network_interface.h"
#include "../include/guicontroller_interface.h"

//class IWindowManager;
using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

class IMainWindow;
using ptr_to_imain_window = std::shared_ptr<IMainWindow>;

class INetworkMainWindow;
using ptr_to_imain_network = std::shared_ptr<INetworkMainWindow>;

class IHandlerMainWindow {
public:
    ~IHandlerMainWindow() = default;
    virtual void setWindowManager(ptr_to_iwindow_manager wind_manager_ptr) = 0;
    virtual void setMainWindow(ptr_to_imain_window main_wind_ptr) = 0;
    virtual void setMainNetwork(ptr_to_imain_network main_net_ptr) = 0;
    virtual void drawPlotHandler(const std::istream& network_output) = 0;
    virtual void stockSelectHandler(const std::string& stock_name) = 0;
    virtual void predictHandler(const std::string& stock_name) = 0;
    virtual void sendError(const Error& error_message) = 0;
    virtual std::string getUrl() = 0;
    virtual void setUrl(const std::string url) = 0;
};

#endif // USECASE_MAINWINDOW_INTERFACE_H
