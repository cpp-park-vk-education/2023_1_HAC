#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

#include "../include/guicontroller.h"
#include <memory>
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    btn_apple = ui->btnApple;
    btn_predict = ui->btnPredict;
    btn_user_set = ui->btnUserSet;
    //widget_graph = ui->widgetGraph;
    ui->widget->xAxis->setRange(0,10);
    ui->widget->yAxis->setRange(0, 10);


    connect(this->get_apple_btn(),SIGNAL(clicked(bool)),this, SLOT(start_apple_plot()));
    connect(this->get_predict_btn(),SIGNAL(clicked(bool)),this, SLOT(start_predict()));
    connect(this->get_user_set_btn(),SIGNAL(clicked(bool)),this, SLOT(open_user_set()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete btn_apple;
    delete btn_predict;
    delete btn_user_set;
    delete errorMes;
    delete error_message_;
    delete error_type_;
}

void MainWindow::start_apple_plot() {
    stock_name = "apple";
    main_handler_ptr->stockSelectHandler("apple");
}

void MainWindow::start_predict() {
    main_handler_ptr->predictHandler(stock_name);
}

void MainWindow::open_user_set() {
    std::cout <<"openning"<<std::endl;
    main_handler_ptr->openUserSettings();
}

QPushButton* MainWindow::get_apple_btn() {
    return btn_apple;
}

QPushButton* MainWindow::get_predict_btn() {
    return btn_predict;
}

QPushButton* MainWindow::get_user_set_btn() {
    return btn_user_set;
}

void MainWindow::drawPlot() {
    ui->widget->clearGraphs();
    QVector<double> x, x_new;
    double max_element =*std::max_element(y.begin(), y.end());
    ui->widget->xAxis->setRange(0, y.size()+2);
    ui->widget->yAxis->setRange(0, max_element+2);
    for (size_t i = 0; i < y.size(); ++i) {
        x.push_back(i);
    }

    ui->widget->xAxis->setLabel("lag");
    ui->widget->yAxis->setLabel("price, $");

    ui->widget->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->widget->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::red));
    ui->widget->graph(0)->addData(x, y);
    ui->widget->graph(0)->setBrush(QBrush(QColor(235, 19, 19, 70)));
    ui->widget->replot();

    /*QLinearGradient lGrad(QPointF(0,0), QPointF(,0));
    lGrad.setColorAt(0, QColor(Qt::red));
    lGrad.setColorAt(1, QColor(Qt::green));
    QBrush lBrush(lGrad);
    QPen myPen;
    myPen.setBrush(lBrush);

    ui->widget->graph(0)->setPen(myPen);
    //ui->widget->graph(0)->setPen(QPen(Qt::red));
    ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();*/
    if (y_new.size() != 0) {
        //ui->widget->graph(0)->setPen(QPen(Qt::green));
        //ui->widget->graph(0)->setBrush(QBrush(QColor(qrand() % 256, qrand() % 256, qrand() % 256, 70)));
        x_new.push_back(y.size()-1);
        y_new.push_front(y[y.size()-1]);
        for (size_t i = 1; i < y_new.size(); ++i) {
            //std::cout <<
            x_new.push_back(y.size() + i);
            std::cout << x_new[i] << ' ';
        }
        //ui->widget->graph(0)->setPen(QPen(Qt::green));

        //ui->widget->addGraph(); // red line
        //ui->widget->graph(1)->setPen(QPen((Qt::green)));
        /*ui->widget->graph(0)->addData(x_new, y_new);
        ui->widget->replot();
        y_new.clear();
        y_new.squeeze();*/
        //ui->widget->update();
        ui->widget->addGraph();
        ui->widget->graph(1)->setPen(QPen(Qt::green));
        ui->widget->graph(1)->addData(x_new, y_new);
        ui->widget->graph(1)->setBrush(QBrush(QColor(162, 242, 136, 70)));
        ui->widget->replot();
    }

    /*ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();
    ui->widget->graph(0)->setPen(QPen(Qt::red));*/

    /*QLinearGradient lGrad(QPointF(0,0), QPointF(x_new[0],y_new[0]));
    lGrad.setColorAt(0, QColor(Qt::red));
    lGrad.setColorAt(1, QColor(Qt::green));
    QBrush lBrush(lGrad);
    QPen myPen;
    myPen.setBrush(lBrush);

    ui->widget->graph(0)->setPen(myPen);*/
    //ui->widget->graph(0)->setPen(QPen(Qt::red));


    //ui->widget->graph(0)->setPen(QPen(Qt::green));

    y_new.clear();
    y_new.squeeze();
    y.clear();
    y.squeeze();
    std::cout << "Graphic is drawn" << std::endl;
}

void MainWindow::setMainWindowHandler(ptr_to_main_handler handler_main_ptr) {
main_handler_ptr = handler_main_ptr;
}

void MainWindow::createPlot(const std::vector<double>& y_plot_data,
                            const std::vector<double>& y_predict_data) {
    y = QVector<double>(y_plot_data.begin(), y_plot_data.end());
    y_new = QVector<double>(y_predict_data.begin(), y_predict_data.end());
}

void MainWindow::showErrorMessage() {
    errorMes = new QErrorMessage(this);
    errorMes->showMessage(*error_type_ + "! " + *error_message_);
    qDebug() << *error_type_ << ' ' << *error_message_;
    std::cout << "error shown"<<std::endl;
}

void MainWindow::createErrorMessage(const Error& error_message) {
    error_type_ = new QString(error_message.type.c_str());
    error_message_ = new QString(error_message.message.c_str());
    std::cout << "created error message"<<std::endl;
}


