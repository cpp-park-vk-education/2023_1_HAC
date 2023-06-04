#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <iostream>

#include <QWidget>
#include <QPushButton>
#include <QVector>
#include <QErrorMessage>
#include <QDebug>
#include <QSignalMapper>
#include <QVector>
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

    QPushButton* getPredictBtn();
    QPushButton* getUserSetBtn();

    void startActions(const std::vector<std::string>& action_strs) override;
    void getActionsData() override;

    void startPlot(const std::string& stock);

public slots:
    void startPredict();
    void openUserSet();

private slots:
    void slotMousePress(QMouseEvent * event);
    void slotMouseMove(QMouseEvent * event);

private:
    Ui::MainWindow *ui;
    ptr_to_main_handler main_handler_ptr;

    QPushButton* btn_predict;
    QPushButton* btn_user_set;
    QErrorMessage* error_mes_;
    QVector<double> y;
    QVector<double> y_new;
    QVector<std::string> x_dates;
    QVector<std::string> x_new_dates;
    QString* error_message_;
    QString* error_type_;

    int predict_parm;

    QVector<QPushButton*> actions;
    int counter;

    QCustomPlot *custom_plot;    // Объявляем графическое полотно
    QCPCurve *verticalLine;     // Объявляем объект для вертикальной линии
    QCPItemTracer *tracer;      // Трасировщик по точкам графика

    std::string stock_name;
    double min_el, max_el, current_x;
};

#endif // MAINWINDOW_H
