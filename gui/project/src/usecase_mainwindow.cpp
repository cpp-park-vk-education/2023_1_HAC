#include "../include/usecase_mainwindow.h"
#include <sstream>

static std::vector<std::string> tokenize(std::string const &str,
                                  const char delim) {
    std::vector<std::string> out;
    std::stringstream ss(str); //строка как поток
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
    return out;
}

void UseCaseMainWindow::setWindowManager(
        ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}

void UseCaseMainWindow::setMainWindow(ptr_to_imain_window main_wind_ptr) {
    main_window_ptr = main_wind_ptr;
}

void UseCaseMainWindow::setMainNetwork(ptr_to_imain_network main_net_ptr) {
    main_network_ptr = main_net_ptr;
}

void UseCaseMainWindow::drawPlotHandler(std::istream& network_output) {
    std::vector<double> new_y;
    std::vector<std::string> new_dates;
    std::string s;
    std::string status;
    std::getline(network_output, status);
    while(std::getline(network_output, s, '\n')) {
        if (s.empty())
            break;
        replaceSubstr(s, ".", ",");
        new_y.push_back(std::stod(s));
    }
    while(std::getline(network_output, s, '\n')) {
        replaceSubstr(s, "\"", "");
        replaceSubstr(s, "\"", "");
        new_dates.push_back(s);
    }
    if (status == "plot") {
        y = new_y;
        dates = new_dates;
        main_window_ptr->createPlot(y, std::vector<double>(), dates, new_dates);
    } else {
        main_window_ptr->createPlot(y, new_y, dates, new_dates);
    }
    main_window_ptr->drawPlot();
    return;
}
void UseCaseMainWindow::stockSelectHandler(const std::string& stock_name) {
    MainData stock_data;
    stock_data.operation_title = "plot";
    stock_data.stock_name = stock_name;
    stock_data.lag = 1000;
    stock_data.window_size = 8;
    main_network_ptr->getPlotData(stock_data);
}
void UseCaseMainWindow::predictHandler(const std::string& stock_name, int wind_size) {
    if (stock_name.empty()) {
        Error error;
        error.type = "NoStock!";
        error.message = "No stock was selected! Select stock and then click predict!";
        main_window_ptr->createErrorMessage(error);
        main_window_ptr->showErrorMessage();
        return;
    }
    stockSelectHandler(stock_name);
    MainData stock_data;
    stock_data.operation_title = "predict";
    stock_data.stock_name = stock_name;
    stock_data.lag = 8;
    stock_data.window_size = wind_size;
    main_network_ptr->getPredictData(stock_data);
}

void UseCaseMainWindow::sendError(const Error& error_message) {
    main_window_ptr->createErrorMessage(error_message);
    main_window_ptr->showErrorMessage();
}


void UseCaseMainWindow::openUserSettings() {
    window_manager_ptr->openUserSettingsWindow();
}

bool UseCaseMainWindow::replaceSubstr(std::string& str, const std::string& from,
                                 const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void  UseCaseMainWindow::getActionsDataHandler() {
    main_network_ptr->getActionsData();
}

void  UseCaseMainWindow::setActionsDataHandler(std::istream& network_output) {
    std::vector<std::string> stocks;
    std::string s;
    while(std::getline(network_output, s, '\n')) {
        replaceSubstr(s, "\"", "");
        replaceSubstr(s, "\"", "");
        stocks.push_back(s);
    }
    main_window_ptr->startActions(stocks);
    return;
}

