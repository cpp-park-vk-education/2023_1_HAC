#ifndef MAINWINDOW_NETWORK_INTERFACE_H
#define MAINWINDOW_NETWORK_INTERFACE_H

#include <memory>

#include "../include/ionetwork_interface.h"
#include "../include/usecase_mainwindow_interface.h"

class IHandlerMainWindow;
using ptr_to_main_handler = std::shared_ptr<IHandlerMainWindow>;

class IONetworkInterface;
using ptr_to_inetwork = std::shared_ptr<IONetworkInterface>;

class INetworkMainWindow {
    virtual void setMainHandler(ptr_to_main_handler handler_main_ptr) = 0;
    virtual void setMainNetwork(ptr_to_inetwork net_ptr) = 0;
    virtual void getPlotData(const MainData& plot_data) = 0;
    virtual void getPredictData(const MainData& plot_data) = 0;
    virtual void onGetPredictData(const std::istream& network_output,
                                  const Error& error_state) = 0;
    virtual void onGetPlotData(const std::istream& network_output,
                               const Error& error_state) = 0;
};

#endif // MAINWINDOW_NETWORK_INTERFACE_H
