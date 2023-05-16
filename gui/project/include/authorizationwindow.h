#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H

#include <iostream>

#include <string>
#include <memory>
#include "../include/authorizationwindow_interface.h"
#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QErrorMessage>

namespace Ui {
class AuthorizationWindow;
}

class AuthorizationWindow : public QWidget, public IAuthorizationWindow {
    Q_OBJECT

public:
    explicit AuthorizationWindow(QWidget *parent = nullptr);
    ~AuthorizationWindow();
    void setAuthorizationWindowHandler(
            ptr_to_authorization_handler auth_handler_ptr) override;
    void showErrorMessage() override;
    void createErrorMessage(const Error& error_message) override;
    std::string getLogin() override;
    std::string getPassword() override;

    QPushButton* get_enter_btn();
    QPushButton* get_reg_btn();

public slots:
    void start_auth();
    void open_reg_window();

private:
    Ui::AuthorizationWindow *ui;
    ptr_to_authorization_handler authorization_handler_ptr;
    std::string login;
    std::string password;
    QPushButton* btn_enter;
    QPushButton* btn_reg;


    QString* error_message_;
    QString* error_type_;
    QErrorMessage* errorMes;
};

#endif // AUTHORIZATIONWINDOW_H
