#include "../include/mainwindow_network.h"

#include <iostream>

void NetworkMainWindow::setMainHandler(ptr_to_main_handler handler_main_ptr) {
    main_handler_ptr = handler_main_ptr;
}
void NetworkMainWindow::setMainNetwork(ptr_to_inetwork net_ptr) {
    network_ptr = net_ptr;
}
void NetworkMainWindow::getPlotData(const MainData& plot_data) {
    std::stringstream info;
    info << "PLOT\n";
    info <<plot_data.operation_title << '\n';
    info <<"name\n" << plot_data.stock_name << '\n';
    info << "lag\n" << plot_data.lag <<'\n';

    std::string r = info.str();
    std::istringstream input(r);
    network_ptr->GetRequest(url_, input,
                            [this](std::istream& network_output,
                                    const Error& error_state)
                                    {onGetPlotData(network_output,
                                                   error_state);});
}
void NetworkMainWindow::getPredictData(const MainData& plot_data) {
    std::stringstream info;
    info << "PREDICT\n";
    info <<plot_data.operation_title << '\n';
    info <<"name\n" << plot_data.stock_name << '\n';
    info << "lenpredict\n" << plot_data.window_size <<'\n';

    std::string str = info.str();
    std::istringstream input(str);
    network_ptr->GetRequest(url_, input,
                            [this](std::istream& network_output,
                                    const Error& error_state)
                                    {onGetPredictData(network_output,
                                                      error_state);});
}
void NetworkMainWindow::onGetPredictData(std::istream& network_output,
                      const Error& error_state) {
    if (error_state.type == "0") {
        std::stringstream output;
        std::string s;
        while(std::getline(network_output, s)) {
            output << s <<'\n';
        }
        std::string network_answer = "predict\n" + output.str();
        std::stringstream new_output;
        new_output <<network_answer;
        main_handler_ptr->drawPlotHandler(new_output);
    } else {
        main_handler_ptr->sendError(error_state);
    }
}
void NetworkMainWindow::onGetPlotData(std::istream& network_output,
                   const Error& error_state) {
    if (error_state.type == "0") {
        std::stringstream output;
        std::string s;
        while(std::getline(network_output, s)) {
            output << s <<'\n';
        }
        std::string network_answer = "plot\n" + output.str();
        std::stringstream new_output;
        new_output <<network_answer;
        main_handler_ptr->drawPlotHandler(new_output);
    } else {
        main_handler_ptr->sendError(error_state);
    }
}
void NetworkMainWindow::setUrl(const std::string& url) {
    url_ = url;
}

void NetworkMainWindow::getActionsData() {
    std::stringstream info;
    info << "NAME_STOCKS\n";
    std::string str = info.str();
    std::istringstream input(str);
    network_ptr->GetRequest(url_, input,
                            [this](std::istream& network_output,
                                    const Error& error_state)
                                    {onGetActionsData(network_output,
                                                      error_state);});
}
void NetworkMainWindow::onGetActionsData(std::istream& network_output,
                                         const Error& error_state) {
    if (error_state.type == "0") {
        std::stringstream output;
        std::string s;
        while(std::getline(network_output, s)) {

            output << s <<'\n';
        }
        main_handler_ptr->setActionsDataHandler(output);
    } else {
        main_handler_ptr->sendError(error_state);
    }
}
