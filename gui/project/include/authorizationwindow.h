#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H

#include <iostream>
#include <string>
#include <memory>

#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QErrorMessage>

#include "../include/authorizationwindow_interface.h"

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

    QPushButton* getEnterBtn();
    QPushButton* getRegBtn();

    void cleanInputLines();

public slots:
    void startAuth();
    void openRegWindow();

private:
    Ui::AuthorizationWindow *ui;
    ptr_to_authorization_handler authorization_handler_ptr;
    std::string login;
    std::string password;
    QPushButton* btn_enter;
    QPushButton* btn_reg;

    QString* error_message_;
    QString* error_type_;
    QErrorMessage* error_mes_;
};

#endif // AUTHORIZATIONWINDOW_H
