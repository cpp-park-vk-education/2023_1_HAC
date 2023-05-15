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
    btn_enter = ui->btnEnter;
    connect(this->get_enter_btn(),SIGNAL(clicked(bool)),this, SLOT(start_auth()));
}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
}

void AuthorizationWindow::setAuthorizationWindowHandler(
        ptr_to_authorization_handler auth_handler_ptr) {
    authorization_handler_ptr = auth_handler_ptr;
}
std::string AuthorizationWindow::getLogin() {
    login = ui->leLogin->text().toStdString();
    return login;
}

std::string AuthorizationWindow::getPassword() {
    password = ui->lePassword->text().toStdString();
    return password;
}


QPushButton* AuthorizationWindow::get_enter_btn() {
    return btn_enter;
}

void AuthorizationWindow::start_auth() {
    //std::string str_log = getLogin();
    //std::cout << getLogin()<<" " <<getPassword()<<std::endl;
    //qDebug() << "1" <<getLogin().c_str() <<"2";
    authorization_handler_ptr->authHandler(getLogin(), getPassword());
}
/*
void AuthorizationWindow::authHandler() {
    //std::string str_log = getLogin();
    //std::cout << getLogin()<<" " <<getPassword()<<std::endl;
    //qDebug() << "1" <<getLogin().c_str() <<"2";
    authorization_handler_ptr->authHandler(getLogin(), getPassword());
}*/

