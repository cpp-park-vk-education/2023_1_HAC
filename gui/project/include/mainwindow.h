#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVector>
#include <QErrorMessage>
#include <QDebug>
#include <vector>
#include <iostream>
#include <QSignalMapper>
#include <QVector>
//#include <QList>

#include <qcustomplot.h>

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
    void createPlot(const std::vector<double>& y_plot_data,
                    const std::vector<double>& y_predict_data,
                    const std::vector<std::string>& x_plot,
                    const std::vector<std::string>& x_predict) override;
    void showErrorMessage() override;
    void createErrorMessage(const Error& error_message) override;

    //QPushButton* get_apple_btn();
    QPushButton* get_predict_btn();
    QPushButton* get_user_set_btn();

    void start_actions(const std::vector<std::string>& action_strs) override;
    void get_actions_data() override;

    void start_plot(const std::string& stock);

public slots:
    //void start_apple_plot();
    void start_predict();
    void open_user_set();

private slots:
    void slotMousePress(QMouseEvent * event);
    void slotMouseMove(QMouseEvent * event);

private:
    Ui::MainWindow *ui;
    ptr_to_main_handler main_handler_ptr;

    //QPushButton* btn_apple;
    QPushButton* btn_predict;
    QPushButton* btn_user_set;
    QErrorMessage* errorMes;
    QVector<double> y;
    QVector<double> y_new;
    QVector<std::string> x_dates;
    QVector<std::string> x_new_dates;
    QString* error_message_;
    QString* error_type_;

    int predict_parm;

    QSignalMapper *mapper;
    QVector<QPushButton*> actions;
    int counter;

    QCustomPlot *customPlot;    // Объявляем графическое полотно
    QCPGraph *graphic;          // Объявляем график
    QCPCurve *verticalLine;     // Объявляем объект для вертикальной линии
    QCPItemTracer *tracer;      // Трасировщик по точкам графика

    std::string stock_name;
    double mi, ma, curenti;
};

#endif // MAINWINDOW_H
