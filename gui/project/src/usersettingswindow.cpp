#include "../include/usersettingswindow.h"
#include "ui_usersettingswindow.h"

#include "../include/guicontroller.h"
#include <memory>
#include <iostream>
#include <string>

UserSettingsWindow::UserSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSettingsWindow)
{
    ui->setupUi(this);
    ui->leOldPassword_2->setEchoMode(QLineEdit::Password);
    ui->leNewPassword_2->setEchoMode(QLineEdit::Password);
    ui->leRepeatPassword_2->setEchoMode(QLineEdit::Password);
    btn_enter = ui->btnEnter;
    btn_return = ui->btnReturn;
    connect(btn_enter,SIGNAL(clicked(bool)),this, SLOT(sendSetting()));
    connect(btn_return,SIGNAL(clicked(bool)),this, SLOT(returnToMain()));

}
UserSettingsWindow::~UserSettingsWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
    delete btn_enter;
    delete btn_return;
    delete errorMes;
}


void UserSettingsWindow::setUserSettingsWindowHandler(ptr_to_settings_handler set_handler_ptr) {
    settings_handler_ptr = set_handler_ptr;
}

void UserSettingsWindow::showErrorMessage() {
    errorMes = new QErrorMessage(this);
    errorMes->showMessage(*error_type_ + "! " + *error_message_);
    qDebug() << *error_type_ << ' ' << *error_message_;
    std::cout << "error shown"<<std::endl;
}

void UserSettingsWindow::createErrorMessage(const Error& error_message) {
    if (error_message.type == "401") {
        error_type_ = new QString("IncorectInput");
        error_message_ = new QString("Incorrect old password was inputed!");
    } else {
        error_type_ = new QString(error_message.type.c_str());
        error_message_ = new QString(error_message.message.c_str());
    }
    std::cout << "created error message"<<std::endl;
}

void UserSettingsWindow::sendSetting() {
    settings_handler_ptr->ConfirmHandler(getOldPassword(), getNewPassword(), getRepeatPassword());
}

void UserSettingsWindow::returnToMain() {
    settings_handler_ptr->passToMain();
}

std::string UserSettingsWindow::getOldPassword() {
    old_password = ui->leOldPassword_2->text().toStdString();
    return old_password;
}

std::string UserSettingsWindow::getNewPassword() {
    new_password = ui->leNewPassword_2->text().toStdString();
    return new_password;
}

std::string UserSettingsWindow::getRepeatPassword() {
    std::string repeat_password = ui->leRepeatPassword_2->text().toStdString();
    return repeat_password;
}
