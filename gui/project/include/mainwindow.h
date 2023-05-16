#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVector>
#include <QErrorMessage>
#include <QDebug>

#include "../include/mainwindow_interface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget, public IMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMainWindowHandler(ptr_to_main_handler handler_main_ptr) override;
    void drawPlot() override;
    void createPlot(const std::vector<double>& y_plot_data) override;
    void showErrorMessage() override;
    void createErrorMessage(const Error& error_message) override;


    QPushButton* get_apple_btn();
    QPushButton* get_predict_btn();
    QPushButton* get_user_set_btn();

public slots:
    void start_apple_plot();
    void start_predict();
    void open_user_set();
private:
    Ui::MainWindow *ui;
    ptr_to_main_handler main_handler_ptr;

    QPushButton* btn_apple;
    QPushButton* btn_predict;
    QPushButton* btn_user_set;
    QErrorMessage* errorMes;
    QVector<double> y;
    QString* error_message_;
    QString* error_type_;


    std::string stock_name;
};

#endif // MAINWINDOW_H
