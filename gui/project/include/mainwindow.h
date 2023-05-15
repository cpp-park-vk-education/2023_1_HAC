#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>

//#include <memory>
#include <string>
#include "../include/mainwindow_interface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget, public IMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMainWindowHandler(ptr_to_main_handler handler_main_ptr) override {
        main_handler_ptr = handler_main_ptr;
    }
    void drawPlot() override {}
    void createPlot(const std::istream& network_output) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {}

    QPushButton* get_apple_btn();
    QPushButton* get_predict_btn();

public slots:
    void start_apple_plot();
    void start_predict();
private:
    Ui::MainWindow *ui;
    ptr_to_main_handler main_handler_ptr;

    QPushButton* btn_apple;
    QPushButton* btn_predict;

    std::string stock_name;

};

#endif // MAINWINDOW_H
