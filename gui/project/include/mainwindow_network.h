#ifndef MAINWINDOW_NETWORK_H
#define MAINWINDOW_NETWORK_H

#include "../include/mainwindow_network_interface.h"
#include <string>
#include <sstream>

class NetworkMainWindow : public INetworkMainWindow {
public:
    void setMainHandler(ptr_to_main_handler handler_main_ptr) override {
        main_handler_ptr = handler_main_ptr;
    }
    void setMainNetwork(ptr_to_inetwork net_ptr) override {
        network_ptr = net_ptr;
    }
    void getPlotData(const MainData& plot_data) override {
        std::cout <<"here I am in PlotNET"<<std::endl;
        network_ptr->GetRequest(url_, plot_data,
                                [this](std::istream& network_output,
                                        const Error& error_state)
                                {onGetPlotData(network_output,
                                               error_state);});
    }
    void getPredictData(const MainData& plot_data) override {
        std::cout <<"here I am in PredictNET"<<std::endl;
        network_ptr->GetRequest(url_, plot_data,
                                [this](std::istream& network_output,
                                        const Error& error_state)
                                {onGetPredictData(network_output,
                                               error_state);});
    }
    void onGetPredictData(std::istream& network_output,
                          const Error& error_state) override {
        std::cout <<"In pregict server answer"<<std::endl;
        std::cout <<"&"<< error_state.type << ' ' <<error_state.message <<"&" << std::endl;
        if (error_state.type == "0") {
            std::stringstream output;
            //char c = ' ';
            std::string s;
            //while (network_output >> c) {
            while(std::getline(network_output, s)) {
                //char c;
                //network_output >> c;
                output << s;
                //std::cout <<"1";
            }
            std::cout<<std::endl;
            std::string network_answer = "predict\n" + output.str();
            output.clear();
            output <<network_answer;
            main_handler_ptr->drawPlotHandler(output);
        } else {
            main_handler_ptr->sendError(error_state);
        }

    }
    void onGetPlotData(std::istream& network_output,
                       const Error& error_state) override {
        std::cout <<"In plot server answer"<<std::endl;
        std::cout << error_state.type << ' ' <<error_state.message << std::endl;
        if (error_state.type == "0") {
            std::stringstream output;
            //char c = ' ';
            std::string s;
            //while (network_output >> c) {
            while(std::getline(network_output, s)) {
                //char c;
                //network_output >> c;
                output << s;
                //std::cout <<"1";
            }
            std::string network_answer = "plot\n" + output.str();
            std::cout <<network_answer<<std::endl;
            output.clear();
            output <<network_answer;
            main_handler_ptr->drawPlotHandler(output);
        } else {
            main_handler_ptr->sendError(error_state);
        }
    }
    void setUrl(const std::string& url) override {
        url_ = url;
    }
private:
    ptr_to_inetwork network_ptr;
    ptr_to_main_handler main_handler_ptr;
    std::string url_;
};

#endif // MAINWINDOW_NETWORK_H
