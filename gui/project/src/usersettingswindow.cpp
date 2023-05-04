#include "../include/usersettingswindow.h"
#include "ui_usersettingswindow.h"

#include "../include/guicontroller.h"
#include <memory>

UserSettingsWindow::UserSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSettingsWindow)
{
    ui->setupUi(this);
}

UserSettingsWindow::~UserSettingsWindow()
{
    delete ui;
}
