#include "../include/mainwindow_network.h"

#include <iostream>

void NetworkMainWindow::setMainHandler(ptr_to_main_handler handler_main_ptr) {
    main_handler_ptr = handler_main_ptr;
}
void NetworkMainWindow::setMainNetwork(ptr_to_inetwork net_ptr) {
    network_ptr = net_ptr;
}
void NetworkMainWindow::getPlotData(const MainData& plot_data) {
    std::cout <<"here I am in PlotNET"<<std::endl;
    network_ptr->GetRequest(url_, plot_data,
                            [this](std::istream& network_output,
                                    const Error& error_state)
                                    {onGetPlotData(network_output,
                                                   error_state);});
}
void NetworkMainWindow::getPredictData(const MainData& plot_data) {
    std::cout <<"here I am in PredictNET"<<std::endl;
    network_ptr->GetRequest(url_, plot_data,
                            [this](std::istream& network_output,
                                    const Error& error_state)
                                    {onGetPredictData(network_output,
                                                      error_state);});
}
void NetworkMainWindow::onGetPredictData(std::istream& network_output,
                      const Error& error_state) {
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
            output << s <<'\n';
            //std::cout <<"1";
        }
        //std::cout<<std::endl;
        std::string network_answer = "predict\n" + output.str();
        //output.clear();
        //output <<network_answer;
        std::stringstream new_output;
        new_output <<network_answer;
        main_handler_ptr->drawPlotHandler(new_output);
    } else {
        main_handler_ptr->sendError(error_state);
    }
}
void NetworkMainWindow::onGetPlotData(std::istream& network_output,
                   const Error& error_state) {
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
            output << s <<'\n';
            //std::cout <<"1";
        }
        std::string network_answer = "plot\n" + output.str();
        std::cout <<"&"<<network_answer<<"&"<<std::endl;
        //output.clear();
        std::stringstream new_output;
        new_output <<network_answer;
        //std::cout <<"!!"<< &output<<"!!";
        /*while(std::getline(new_output, s)) {
            std::cout <<"("<< s << ' ';
        }*/
        main_handler_ptr->drawPlotHandler(new_output);
    } else {
        main_handler_ptr->sendError(error_state);
    }
}
void NetworkMainWindow::setUrl(const std::string& url) {
    url_ = url;
}
