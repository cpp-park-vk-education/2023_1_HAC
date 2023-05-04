#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H

#include <string>
#include <memory>
#include "../include/authorizationwindow_interface.h"
#include <QWidget>

namespace Ui {
class AuthorizationWindow;
}

class AuthorizationWindow : public QWidget, public IAuthorizationWindow {
    Q_OBJECT

public:
    explicit AuthorizationWindow(QWidget *parent = nullptr);
    ~AuthorizationWindow();
    void setAuthorizationWindowHandler(
            ptr_to_authorization_handler auth_handler_ptr) override {};
    void showErrorMessage() override {};
    void createErrorMessage(const Error& error_message) override {};
    std::string getLogin() override {return {};};
    std::string getPassword() override {return {};};

private:
    Ui::AuthorizationWindow *ui;
    std::shared_ptr<IONetworkInterface> network_ptr;
    ptr_to_authorization_handler authorization_handler_ptr;
};

#endif // AUTHORIZATIONWINDOW_H
