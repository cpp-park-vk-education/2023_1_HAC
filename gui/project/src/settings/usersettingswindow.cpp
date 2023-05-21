#include "../../include/settings/usersettingswindow.h"
#include "ui_usersettingswindow.h"

#include "../../include/guicontroller.h"
#include <memory>

UserSettingsWindow::UserSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSettingsWindow)
{
    ui->setupUi(this);
    //btn_enter = ui->btnEnter;
    btn_password = ui->btnPassword;
    btn_email = ui->btnEmail;
    btn_return = ui->btnReturn;
    connect(btn_password,SIGNAL(clicked(bool)),this, SLOT(PasswordChange()));
    connect(btn_email,SIGNAL(clicked(bool)),this, SLOT(EmailChange()));
    connect(btn_return,SIGNAL(clicked(bool)),this, SLOT(returnToMain()));

}

UserSettingsWindow::~UserSettingsWindow()
{
    delete ui;
}


void UserSettingsWindow::setUserSettingsWindowHandler(ptr_to_settings_handler set_handler_ptr) {
    settings_handler_ptr = set_handler_ptr;
}

void UserSettingsWindow::returnToMain() {
    settings_handler_ptr->passToMain();
}

void UserSettingsWindow::PasswordChange() {
    settings_handler_ptr->OpenPasswordWindow();
}

void UserSettingsWindow::EmailChange() {
    settings_handler_ptr->OpenEmailWindow();
}

