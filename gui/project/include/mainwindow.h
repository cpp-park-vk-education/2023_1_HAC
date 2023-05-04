#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
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
    void setMainWindowHandler(ptr_to_main_handler handler_main_ptr) override {}
    void drawPlot() override {}
    void createPlot(const std::istream& network_output) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {}

private:
    Ui::MainWindow *ui;
    ptr_to_main_handler main_handler_ptr;
};

#endif // MAINWINDOW_H
