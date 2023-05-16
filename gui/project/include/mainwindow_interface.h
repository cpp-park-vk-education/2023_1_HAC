#ifndef MAINWINDOW_INTERFACE_H
#define MAINWINDOW_INTERFACE_H

#include "../include/usecase_mainwindow_interface.h"
#include "../include/ionetwork_interface.h"

class IHandlerMainWindow;
using ptr_to_main_handler = std::shared_ptr<IHandlerMainWindow>;

class IMainWindow {
public:
    ~IMainWindow() = default;
    virtual void setMainWindowHandler(ptr_to_main_handler handler_main_ptr) = 0;
    virtual void drawPlot() = 0;
    virtual void createPlot(const std::vector<double>& y_plot_data) = 0;
    virtual void showErrorMessage() = 0;
    virtual void createErrorMessage(const Error& error_message) = 0;
};

#endif // MAINWINDOW_INTERFACE_H
