#include "../include/authorizationwindow.h"
#include "ui_authorizationwindow.h"
#include "../include/authorizationwindow_interface.h"

#include "../include/guicontroller_interface.h"

//#include <memory>

AuthorizationWindow::AuthorizationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);
}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
}
