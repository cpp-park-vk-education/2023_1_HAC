#include "include/loadingwindow.h"
#include "ui_loadingwindow.h"

#include <QSplashScreen>
#include <QTime>
#include <QElapsedTimer>

#include <iostream>
#include <chrono>
#include <thread>

static const int LOAD_TIME_MSEC = 5 * 1000;

LoadingWindow::LoadingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingWindow)
{
    ui->setupUi(this);

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(29,30,51));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    image_label = ui->label;
    progress = ui->progressBar;

    QPalette p = progress->palette();
    p.setColor(QPalette::Highlight, QColor(56, 27, 161));
    progress->setPalette(p);

    //progress->setCo
    //QPixmap image("/home/sofi_flin/Desktop/DB/MMM/gui/project/load.png");
    //QPixmap image("../../project/load.png");
    QPixmap image("../../project/loading.png");
    QPixmap pixmapScaled = image.scaled(QSize(300,500),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    image_label->setPixmap(pixmapScaled);
    percent = 1;
}

LoadingWindow::~LoadingWindow()
{
    delete ui;
}

void LoadingWindow::perform_progress() {
    if (percent <= 100) {
        ui->progressBar->setValue(percent);
        percent++;
    } else {
        percent = 1;
        window_manager_ptr->stop_timer();
    }
}
