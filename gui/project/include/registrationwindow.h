#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>

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
            ptr_to_registration_handler reg_handler_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {}
    std::string getLogin() override {}
    std::string getPassword() override {}
    std::string getEmail() override {}
private:
    Ui::RegistrationWindow *ui;
    ptr_to_registration_handler registration_handler_ptr;
};

#endif // REGISTRATIONWINDOW_H
