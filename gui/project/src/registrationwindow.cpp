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
    ui->lePassword->setEchoMode(QLineEdit::Password);
    ui->lePassConfirm->setEchoMode(QLineEdit::Password);
    btn_enter = ui->btnEnter;
    btn_back = ui->btnBack;

    connect(this->get_enter_btn(),SIGNAL(clicked(bool)),this, SLOT(start_reg()));
    connect(this->get_back_btn(),SIGNAL(clicked(bool)),this, SLOT(open_auth_window()));
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
    delete errorMes;
    delete btn_enter;
    delete btn_back;
}

void RegistrationWindow::setRegistrationWindowHandler(
        ptr_to_registration_handler reg_handler_ptr) {
    registration_handler_ptr = reg_handler_ptr;
}

void RegistrationWindow::showErrorMessage() {
    errorMes = new QErrorMessage(this);
    errorMes->showMessage(*error_type_ + "! " + *error_message_);
    qDebug() << *error_type_ << ' ' << *error_message_;
    std::cout << "error shown"<<std::endl;
}

void RegistrationWindow::createErrorMessage(const Error& error_message) {
    error_type_ = new QString(error_message.type.c_str());
    error_message_ = new QString(error_message.message.c_str());
    std::cout << "created error message"<<std::endl;
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

QPushButton* RegistrationWindow::get_enter_btn() {
    return btn_enter;
}

QPushButton* RegistrationWindow::get_back_btn() {
    return btn_back;
}

void RegistrationWindow::start_reg() {
    std::cout << "In start reg" <<std::endl;
    registration_handler_ptr->regHandler(getEmail(), getLogin(), getPassword(), getPasswordConfirm());
}

void RegistrationWindow::open_auth_window() {
    std::cout << "In start reg" <<std::endl;
    registration_handler_ptr->openAuthorizationWindow();
}

