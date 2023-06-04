#include "../include/authorizationwindow.h"
#include "ui_authorizationwindow.h"
#include "../include/authorizationwindow_interface.h"

#include "../include/guicontroller_interface.h"

AuthorizationWindow::AuthorizationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);

    //background color
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(29,30,51));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //Hide password with special dots
    ui->lePassword->setEchoMode(QLineEdit::Password);

    btn_enter = ui->btnEnter;
    btn_reg = ui->btnReg;

    ui->lePassword->setStyleSheet("background-color:rgb(200,201,182);");
    ui->lePassword->setText("");
    ui->leLogin->setStyleSheet("background-color:rgb(200,201,182);");
    ui->leLogin->setText("");

    connect(this->getEnterBtn(),SIGNAL(clicked(bool)),this, SLOT(startAuth()));
    connect(this->getRegBtn(),SIGNAL(clicked(bool)),this, SLOT(openRegWindow()));
}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
    delete error_mes_;
    delete btn_enter;
    delete btn_reg;
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

QPushButton* AuthorizationWindow::getEnterBtn() {
    return btn_enter;
}

QPushButton* AuthorizationWindow::getRegBtn() {
    return btn_reg;
}

void AuthorizationWindow::startAuth() {
    authorization_handler_ptr->authHandler(getLogin(), getPassword());
}

void AuthorizationWindow::openRegWindow() {
    authorization_handler_ptr->openRegistrationWindow();
}

void AuthorizationWindow::showErrorMessage() {
    error_mes_ = new QErrorMessage(this);
    error_mes_->setWindowTitle("Error!");
    error_mes_->showMessage(*error_type_ + "! " + *error_message_);
}

void AuthorizationWindow::createErrorMessage(const Error& error_message) {
    if (error_message.type == "401") {
        error_type_ = new QString("IncorectInput");
        error_message_ = new QString("Incorrect login or password!");
    } else {
        error_type_ = new QString(error_message.type.c_str());
        error_message_ = new QString(error_message.message.c_str());
    }
}

void AuthorizationWindow::cleanInputLines() {
    ui->lePassword->setText("");
    ui->leLogin->setText("");
}
