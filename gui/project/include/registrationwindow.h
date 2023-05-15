#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include <QErrorMessage>
#include <QDebug>

#include <iostream>

#include "../include/registrationwindow_interface.h"
//#include "../include/usecase_registrationwindow_interface.h"

class IHandlerRegistrationWindow;
using ptr_to_registration_handler = std::shared_ptr<IHandlerRegistrationWindow>;

namespace Ui {
class RegistrationWindow;
}

class RegistrationWindow : public QWidget, public IRegistrationWindow
{
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);
    ~RegistrationWindow();
    void setRegistrationWindowHandler(
            ptr_to_registration_handler reg_handler_ptr) override {
        registration_handler_ptr = reg_handler_ptr;
    }
    void showErrorMessage() override {
        errorMes = new QErrorMessage(this);
        errorMes->showMessage(*error_type_ + "! " + *error_message_);
        qDebug() << *error_type_ << ' ' << *error_message_;
        std::cout << "error shown"<<std::endl;
    }
    void createErrorMessage(const Error& error_message) override {
        error_type_ = new QString(error_message.type.c_str());
        error_message_ = new QString(error_message.message.c_str());
        std::cout << "created error message"<<std::endl;
    }
    std::string getLogin() override;
    std::string getPassword() override;
    std::string getPasswordConfirm() override;
    std::string getEmail() override;

    QPushButton* get_enter_btn();

public slots:
    void start_reg();
private:
    Ui::RegistrationWindow *ui;
    ptr_to_registration_handler registration_handler_ptr;

    std::string login;
    std::string password;
    std::string email;

    QString* error_message_;
    QString* error_type_;
    QErrorMessage* errorMes;

    QPushButton* btn_enter;
};

#endif // REGISTRATIONWINDOW_H
