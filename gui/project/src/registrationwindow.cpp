#include "../include/registrationwindow.h"
#include "ui_registrationwindow.h"

#include "../include/guicontroller.h"
#include <memory>

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
    btn_enter = ui->btnEnter;
    connect(this->get_enter_btn(),SIGNAL(clicked(bool)),this, SLOT(start_reg()));

}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
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
    password = ui->lePassCpnfirm->text().toStdString();
    return password;
}

std::string RegistrationWindow::getEmail() {
    email= ui->leEmail->text().toStdString();
    return password;
}

QPushButton* RegistrationWindow::get_enter_btn() {
    return btn_enter;
}

void RegistrationWindow::start_reg() {
    std::cout << "In start reg" <<std::endl;
    registration_handler_ptr->regHandler(getEmail(), getLogin(), getPassword(), getPasswordConfirm());
}

