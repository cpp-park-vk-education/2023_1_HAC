#include "../../include/settings/usersettingswindow.h"
#include "ui_usersettingswindow.h"
#include "../../include/guicontroller.h"

UserSettingsWindow::UserSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserSettingsWindow)
{
    ui->setupUi(this);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(29,30,51));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    
    btn_password = ui->btnPassword;
    btn_email = ui->btnEmail;
    btn_return = ui->btnReturn;
    btn_exit = ui->btnExit;
    connect(btn_password,SIGNAL(clicked(bool)),this, SLOT(passwordChange()));
    connect(btn_email,SIGNAL(clicked(bool)),this, SLOT(emailChange()));
    connect(btn_return,SIGNAL(clicked(bool)),this, SLOT(returnToMain()));
    connect(btn_exit,SIGNAL(clicked(bool)),this, SLOT(startExiting()));

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

void UserSettingsWindow::passwordChange() {
    settings_handler_ptr->openPasswordWindow();
}

void UserSettingsWindow::emailChange() {
    settings_handler_ptr->openEmailWindow();
}

void UserSettingsWindow::startExiting() {
    settings_handler_ptr->userExitHandler();
}

