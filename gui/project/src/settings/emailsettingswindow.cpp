#include "../../include/settings/emailsettingswindow.h"
#include "ui_emailsettingswindow.h"

#include "../../include/guicontroller.h"
#include <memory>
#include <iostream>

EmailSettingsWindow::EmailSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmailSettingsWindow)
{
    ui->setupUi(this);
    btn_enter = ui->btnEnter;
    btn_return = ui->btnReturn;
    connect(btn_enter,SIGNAL(clicked(bool)),this, SLOT(sendSetting()));
    connect(btn_return,SIGNAL(clicked(bool)),this, SLOT(returnToMain()));

}

EmailSettingsWindow::~EmailSettingsWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
    delete btn_enter;
    delete btn_return;
    delete errorMes;
}


void EmailSettingsWindow::setEmailSettingsWindowHandler(ptr_to_emailsettings_handler set_handler_ptr) {
    settings_handler_ptr = set_handler_ptr;
}

void EmailSettingsWindow::showErrorMessage() {
    errorMes = new QErrorMessage(this);
    errorMes->showMessage(*error_type_ + "! " + *error_message_);
    qDebug() << *error_type_ << ' ' << *error_message_;
    std::cout << "error shown"<<std::endl;
    std::cout << "created error message"<<std::endl;
}

void EmailSettingsWindow::createErrorMessage(const Error& error_message) {
    if (error_message.type == "401") {
        error_type_ = new QString("IncorectInput");
        error_message_ = new QString("Incorrect old password was inputed!");
    } else {
        error_type_ = new QString(error_message.type.c_str());
        error_message_ = new QString(error_message.message.c_str());
    }
    std::cout << "created error message"<<std::endl;
}

void EmailSettingsWindow::sendSetting() {
    settings_handler_ptr->ConfirmHandler(getNewEmail());
}

void EmailSettingsWindow::returnToMain() {
    settings_handler_ptr->passToMain();
}

std::string EmailSettingsWindow::getNewEmail() {
    new_email = ui->leNewEmail->text().toStdString();
    return new_email;
}
