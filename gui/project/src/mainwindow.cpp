#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

#include "../include/guicontroller.h"
#include <memory>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    btn_apple = ui->btnApple;
    btn_predict = ui->btnPredict;

    connect(this->get_apple_btn(),SIGNAL(clicked(bool)),this, SLOT(start_apple_plot()));
    connect(this->get_predict_btn(),SIGNAL(clicked(bool)),this, SLOT(start_predict()));
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

QPushButton* MainWindow::get_apple_btn() {
    return btn_apple;
}

QPushButton* MainWindow::get_predict_btn() {
    return btn_predict;
}
