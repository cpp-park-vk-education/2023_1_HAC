#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include <QErrorMessage>
#include <QDebug>

#include "../include/registrationwindow_interface.h"


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
            ptr_to_registration_handler reg_handler_ptr) override;
    void showErrorMessage() override;
    void createErrorMessage(const Error& error_message) override;
    std::string getLogin() override;
    std::string getPassword() override;
    std::string getPasswordConfirm() override;
    std::string getEmail() override;

    QPushButton* get_enter_btn();
    QPushButton* get_back_btn();

public slots:
    void start_reg();
    void open_auth_window();
private:
    Ui::RegistrationWindow *ui;
    ptr_to_registration_handler registration_handler_ptr;

    std::string login;
    std::string password;
    std::string pass_confirm;
    std::string email;

    QString* error_message_;
    QString* error_type_;
    QErrorMessage* errorMes;

    QPushButton* btn_enter;
    QPushButton* btn_back;
};

#endif // REGISTRATIONWINDOW_H
