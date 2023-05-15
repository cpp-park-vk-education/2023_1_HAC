#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

#include "../include/guicontroller.h"
#include <memory>
#include <iostream>

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
}

void MainWindow::start_apple_plot() {
    stock_name = "Apple";
    main_handler_ptr->stockSelectHandler("Apple");
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
    //ui->widgetGraph
    ui->widget->clearGraphs();
    QVector<double> x;
    double max_element =*std::max_element(y.begin(), y.end());
    ui->widget->xAxis->setRange(0, y.size()+2);
    ui->widget->yAxis->setRange(0, max_element+2);
    for (size_t i = 0; i < y.size(); ++i) {
        x.push_back(i);
    }
    ui->widget->addGraph();
    ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();
    y.clear();
    std::cout << "Graphic is drawn" << std::endl;
}
