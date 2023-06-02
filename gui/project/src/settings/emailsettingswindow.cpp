#include "../../include/settings/emailsettingswindow.h"
#include "ui_emailsettingswindow.h"
#include "../../include/guicontroller.h"


EmailSettingsWindow::EmailSettingsWindow(QWidget *parent) : QWidget(parent), ui(new Ui::EmailSettingsWindow)
{
    ui->setupUi(this);

    //background color
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(29,30,51));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    ui->lePassword->setEchoMode(QLineEdit::Password);
    ui->lePassword->setStyleSheet("background-color:rgb(200,201,182);");
    ui->leNewEmail->setStyleSheet("background-color:rgb(200,201,182);");

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
    delete error_mes_;
}


void EmailSettingsWindow::setEmailSettingsWindowHandler(ptr_to_emailsettings_handler set_handler_ptr) {
    settings_handler_ptr = set_handler_ptr;
}


void EmailSettingsWindow::showErrorMessage() {
    error_mes_ = new QErrorMessage(this);
    error_mes_->setWindowTitle("Error!");
    error_mes_->showMessage(*error_type_ + "! " + *error_message_);
    qDebug() << *error_type_ << ' ' << *error_message_;
}


void EmailSettingsWindow::createErrorMessage(const Error& error_message) {
    if (error_message.type == "401") {
        error_type_ = new QString("IncorectInput");
        error_message_ = new QString("Incorrect old password was inputed!");
    } else {
        error_type_ = new QString(error_message.type.c_str());
        error_message_ = new QString(error_message.message.c_str());
    }
}


void EmailSettingsWindow::sendSetting() {
    settings_handler_ptr->confirmHandler(getNewEmail(), getPassword());
}


void EmailSettingsWindow::returnToMain() {
    settings_handler_ptr->passToMain();
}


std::string EmailSettingsWindow::getNewEmail() {
    new_email = ui->leNewEmail->text().toStdString();
    return new_email;
}

std::string EmailSettingsWindow::getPassword() {
    password = ui->lePassword->text().toStdString();
    return password;
}

void EmailSettingsWindow::cleanInputLines() {
    ui->lePassword->setText("");
    ui->leNewEmail->setText("");
}
