#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include "../include/guicontroller_interface.h"

using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

namespace Ui {
class LoadingWindow;
}

class LoadingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoadingWindow(QWidget *parent = nullptr);
    ~LoadingWindow();
    void setWindowManager(
            ptr_to_iwindow_manager wind_manager_ptr) {
        window_manager_ptr = wind_manager_ptr;
    }

public slots:
    void perform_progress();

private:
    Ui::LoadingWindow *ui;
    QLabel* image_label;
    QProgressBar* progress;
    int percent;

    ptr_to_iwindow_manager window_manager_ptr;
};

#endif // LOADINGWINDOW_H
