#ifndef MAINWINDOW_NETWORK_H
#define MAINWINDOW_NETWORK_H

#include "../include/mainwindow_network_interface.h"

class NetworkMainWindow : public INetworkMainWindow {
public:
    void setMainHandler(ptr_to_main_handler handler_main_ptr) override {}
    void setMainNetwork(ptr_to_inetwork net_ptr) override {}
    void getPlotData(const MainData& plot_data) override {}
    void getPredictData(const MainData& plot_data) override {}
    void onGetPredictData(std::istream& network_output,
                          const Error& error_state) override {}
    void onGetPlotData(std::istream& network_output,
                       const Error& error_state) override {}
    void setUrl(const std::string& url) override {}
private:
    ptr_to_inetwork network_ptr;
    ptr_to_main_handler main_handler_ptr;
};

#endif // MAINWINDOW_NETWORK_H
