#include "../include/registrationwindow.h"
#include "ui_registrationwindow.h"

#include "../include/guicontroller.h"
#include <memory>
#include <iostream>

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(29,30,51));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //цвет фона
    ui->lePassword->setStyleSheet("background-color:rgb(200,201,182);");
    ui->lePassConfirm->setStyleSheet("background-color:rgb(200,201,182);");
    ui->leLogin->setStyleSheet("background-color:rgb(200,201,182);");
    ui->leEmail->setStyleSheet("background-color:rgb(200,201,182);");

    ui->lePassword->setEchoMode(QLineEdit::Password);
    ui->lePassConfirm->setEchoMode(QLineEdit::Password);
    btn_enter = ui->btnEnter;
    btn_back = ui->btnBack;

    ui->lePassword->setText("");
    ui->leLogin->setText("");
    ui->lePassConfirm->setText("");
    ui->leEmail->setText("");

    connect(this->getEnterBtn(),SIGNAL(clicked(bool)),this, SLOT(startReg()));
    connect(this->getBackBtn(),SIGNAL(clicked(bool)),this, SLOT
    (openAuthWindow()));
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
    delete error_mes_;
    delete btn_enter;
    delete btn_back;
}

void RegistrationWindow::setRegistrationWindowHandler(
        ptr_to_registration_handler reg_handler_ptr) {
    registration_handler_ptr = reg_handler_ptr;
}

void RegistrationWindow::showErrorMessage() {
    error_mes_ = new QErrorMessage(this);
    error_mes_->setWindowTitle("Error!");
    error_mes_->showMessage(*error_type_ + "! " + *error_message_);
}

void RegistrationWindow::createErrorMessage(const Error& error_message) {
    error_type_ = new QString(error_message.type.c_str());
    error_message_ = new QString(error_message.message.c_str());
}

std::string RegistrationWindow::getLogin() {
    login = ui->leLogin->text().toStdString();
    return login;
}

std::string RegistrationWindow::getPassword() {
    password = ui->lePassword->text().toStdString();
    return password;
}

std::string RegistrationWindow::getPasswordConfirm() {
    pass_confirm = ui->lePassConfirm->text().toStdString();
    return pass_confirm;
}

std::string RegistrationWindow::getEmail() {
    email= ui->leEmail->text().toStdString();
    return email;
}

QPushButton* RegistrationWindow::getEnterBtn() {
    return btn_enter;
}

QPushButton* RegistrationWindow::getBackBtn() {
    return btn_back;
}

void RegistrationWindow::startReg() {
    registration_handler_ptr->regHandler(getEmail(), getLogin(), getPassword(), getPasswordConfirm());
}

void RegistrationWindow::openAuthWindow() {
    registration_handler_ptr->openAuthorizationWindow();
}
void RegistrationWindow::cleanInputLines() {
    ui->lePassword->setText("");
    ui->leLogin->setText("");
    ui->lePassConfirm->setText("");
    ui->leEmail->setText("");
}

