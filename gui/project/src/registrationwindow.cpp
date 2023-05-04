#include "../include/registrationwindow.h"
#include "ui_registrationwindow.h"

#include "../include/guicontroller.h"
#include <memory>

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}
