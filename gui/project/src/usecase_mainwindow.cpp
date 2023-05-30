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
    //распарсить
    //setlocale(LC_ALL,"en_us");
    std::vector<double> new_y;
    std::vector<std::string> new_dates;
    std::string s;
    std::string status;
    std::getline(network_output, status);
    std::cout << "*" <<status <<"*" <<std::endl;
    while(std::getline(network_output, s, '\n')) {
        if (s.empty())
            break;
        replace(s, ".", ",");
        //std::string a ="100.1";
        std::cout << "^" <<std::stod(s)<<"^";
        new_y.push_back(std::stod(s));
    }
    while(std::getline(network_output, s, '\n')) {
        replace(s, "\"", "");
        replace(s, "\"", "");
        std::cout << "^" << s <<"^";
        new_dates.push_back(s);
        //new_y.push_back(s);
    }
    std::cout <<"pred dates size = " <<new_dates.size() <<"!" <<std::endl;
    /*if (status == "predict") {
        //y.insert(y.end(), new_y.begin(), new_y.end());
    } else {
    y = new_y;
    //new_y.clear();
    new_y.erase(new_y.begin(), new_y.end());
    }*/
    if (status == "plot") {
        y = new_y;
        dates = new_dates;
        std::cout <<"size dates = " << dates.size() <<std::endl;
        //new_y.clear();
       // new_y.erase(new_y.begin(), new_y.end());
        main_window_ptr->createPlot(y, std::vector<double>(), dates, new_dates);
    } else {
        main_window_ptr->createPlot(y, new_y, dates, new_dates);
    }
    //main_window_ptr->createPlot(y, new_y);
    main_window_ptr->drawPlot();
    return;
}
void UseCaseMainWindow::stockSelectHandler(const std::string& stock_name) {
    std::cout << "In stock selected: " <<stock_name<< std::endl;
    //stock_name_ = stock_name;
    MainData stock_data;
    stock_data.operation_title = "plot";
    stock_data.stock_name = stock_name;
    //stock_data.lag = 7 * 24;
    stock_data.lag = 1000;
    stock_data.window_size = 8;
    main_network_ptr->getPlotData(stock_data);
}
void UseCaseMainWindow::predictHandler(const std::string& stock_name, int wind_size) {
    std::cout << "In predict Handler: " <<stock_name<< std::endl;
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
    std::cout <<"handler" <<std::endl;
    window_manager_ptr->openUserSettingsWindow();
}

bool UseCaseMainWindow::replace(std::string& str, const std::string& from, const std::string& to) {
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
        replace(s, "\"", "");
        replace(s, "\"", "");
        stocks.push_back(s);
    }
    main_window_ptr->start_actions(stocks);
    return;
}

