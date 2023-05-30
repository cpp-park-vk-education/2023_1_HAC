#include "../include/authorizationwindow.h"
#include "ui_authorizationwindow.h"
#include "../include/authorizationwindow_interface.h"

#include "../include/guicontroller_interface.h"

AuthorizationWindow::AuthorizationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);

    //background color
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, QColor(29,30,51));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    ui->lePassword->setEchoMode(QLineEdit::Password);
    btn_enter = ui->btnEnter;
    btn_reg = ui->btnReg;

    ui->lePassword->setStyleSheet("background-color:rgb(200,201,182);");
    ui->lePassword->setText("");
    ui->leLogin->setStyleSheet("background-color:rgb(200,201,182);");
    ui->leLogin->setText("");

    connect(this->get_enter_btn(),SIGNAL(clicked(bool)),this, SLOT(start_auth()));
    connect(this->get_reg_btn(),SIGNAL(clicked(bool)),this, SLOT(open_reg_window()));

}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
    delete error_message_;
    delete error_type_;
    delete errorMes;
    delete btn_enter;
    delete btn_reg;
}

void AuthorizationWindow::setAuthorizationWindowHandler(
        ptr_to_authorization_handler auth_handler_ptr) {
    authorization_handler_ptr = auth_handler_ptr;
}
std::string AuthorizationWindow::getLogin() {
    login = ui->leLogin->text().toStdString();
    return login;
}

std::string AuthorizationWindow::getPassword() {
    password = ui->lePassword->text().toStdString();
    return password;
}


QPushButton* AuthorizationWindow::get_enter_btn() {
    return btn_enter;
}

QPushButton* AuthorizationWindow::get_reg_btn() {
    return btn_reg;
}


void AuthorizationWindow::start_auth() {
    authorization_handler_ptr->authHandler(getLogin(), getPassword());
}

void AuthorizationWindow::open_reg_window() {
    authorization_handler_ptr->openRegistrationWindow();
    //ui->lePassword->setText("");
    //ui->leLogin->setText("");
}

void AuthorizationWindow::showErrorMessage() {
    errorMes = new QErrorMessage(this);
    errorMes->showMessage(*error_type_ + "! " + *error_message_);
    qDebug() << *error_type_ << ' ' << *error_message_;
    std::cout << "error shown"<<std::endl;
}

void AuthorizationWindow::createErrorMessage(const Error& error_message) {
    if (error_message.type == "401") {
        error_type_ = new QString("IncorectInput");
        error_message_ = new QString("Incorrect login or password!");
    } else {
        error_type_ = new QString(error_message.type.c_str());
        error_message_ = new QString(error_message.message.c_str());
    }
    std::cout << "created error message"<<std::endl;
}

void AuthorizationWindow::clean_input_lines() {
    ui->lePassword->setText("");
    ui->leLogin->setText("");
}
