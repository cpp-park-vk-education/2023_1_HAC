#include "../../include/settings/passwordsettingswindow.h"
#include "ui_passwordsettingswindow.h"

#include "../../include/guicontroller.h"
#include <memory>
#include <iostream>

PasswordSettingsWindow::PasswordSettingsWindow(QWidget *parent) : QWidget(parent), ui(new Ui::PasswordSettingsWindow)
{
    ui->setupUi(this);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(29,30,51));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    ui->leOldPassword_2->setEchoMode(QLineEdit::Password);
    ui->leNewPassword_2->setEchoMode(QLineEdit::Password);
    ui->leRepeatPassword_2->setEchoMode(QLineEdit::Password);

    ui->leOldPassword_2->setStyleSheet("background-color:rgb(200,201,182);");
    ui->leNewPassword_2->setStyleSheet("background-color:rgb(200,201,182);");
    ui->leRepeatPassword_2->setStyleSheet("background-color:rgb(200,201,182);");

    btn_enter = ui->btnEnter;
    btn_return = ui->btnReturn;
    connect(btn_enter,SIGNAL(clicked(bool)),this, SLOT(sendSetting()));
    connect(btn_return,SIGNAL(clicked(bool)),this, SLOT(returnToMain()));

}

PasswordSettingsWindow::~PasswordSettingsWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
    delete btn_enter;
    delete btn_return;
    delete errorMes;
}


void PasswordSettingsWindow::setPasswordSettingsWindowHandler(ptr_to_passwordsettings_handler set_handler_ptr) {
    settings_handler_ptr = set_handler_ptr;
}

void PasswordSettingsWindow::showErrorMessage() {
    errorMes = new QErrorMessage(this);
    errorMes->setWindowTitle("Error!");
    errorMes->showMessage(*error_type_ + "! " + *error_message_);
    qDebug() << *error_type_ << ' ' << *error_message_;
    std::cout << "error shown"<<std::endl;
}

void PasswordSettingsWindow::createErrorMessage(const Error& error_message) {
    if (error_message.type == "401") {
        error_type_ = new QString("IncorectInput");
        error_message_ = new QString("Incorrect old password was inputed!");
    } else {
        error_type_ = new QString(error_message.type.c_str());
        error_message_ = new QString(error_message.message.c_str());
    }
    std::cout << "created error message"<<std::endl;
}

void PasswordSettingsWindow::sendSetting() {
    settings_handler_ptr->ConfirmHandler(getOldPassword(), getNewPassword(), getRepeatPassword());
}

void PasswordSettingsWindow::returnToMain() {
    settings_handler_ptr->passToMain();
}

std::string PasswordSettingsWindow::getOldPassword() {
    old_password = ui->leOldPassword_2->text().toStdString();
    return old_password;
}

std::string PasswordSettingsWindow::getNewPassword() {
    new_password = ui->leNewPassword_2->text().toStdString();
    return new_password;
}

std::string PasswordSettingsWindow::getRepeatPassword() {
    std::string repeat_password = ui->leRepeatPassword_2->text().toStdString();
    return repeat_password;
}

void PasswordSettingsWindow::clean_input_lines() {
    ui->leOldPassword_2->setText("");
    ui->leNewPassword_2->setText("");
    ui->leRepeatPassword_2->setText("");
}
