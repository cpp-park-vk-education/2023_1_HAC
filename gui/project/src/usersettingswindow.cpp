#include "../include/usersettingswindow.h"
#include "ui_usersettingswindow.h"

#include "../include/guicontroller.h"
#include <memory>

UserSettingsWindow::UserSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSettingsWindow)
{
    ui->setupUi(this);
    btn_enter = ui->btnEnter;
    connect(btn_enter,SIGNAL(clicked(bool)),this, SLOT(send_setting()));
}
UserSettingsWindow::~UserSettingsWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
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
    error_type_ = new QString(error_message.type.c_str());
    error_message_ = new QString(error_message.message.c_str());
    std::cout << "created error message"<<std::endl;
}

void UserSettingsWindow::send_setting() {
    settings_handler_ptr->ConfirmHandler(getOldPassword(), getNewPassword(), getRepeatPassword());
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
