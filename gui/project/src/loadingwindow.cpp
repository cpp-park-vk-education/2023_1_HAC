#include "include/loadingwindow.h"
#include "ui_loadingwindow.h"

#include <QSplashScreen>
#include <QTime>
#include <QElapsedTimer>

static const QString PATH_TO_PICTURE = "../../project/loading.png";
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

    QPixmap image(PATH_TO_PICTURE);
    QPixmap pixmapScaled = image.scaled(QSize(300,500),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    image_label->setPixmap(pixmapScaled);
    percent = 1;
}

LoadingWindow::~LoadingWindow()
{
    delete ui;
    delete image_label;
}

void LoadingWindow::setWindowManager(
        ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
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
