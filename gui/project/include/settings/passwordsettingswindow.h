#ifndef PASSWORDSETTINGSWINDOW_H
#define PASSWORDSETTINGSWINDOW_H

#include <QWidget>
#include <memory>
#include <QErrorMessage>
#include <QDebug>

#include "passwordsettingswindow_interface.h"

namespace Ui {
    class PasswordSettingsWindow;
}

class PasswordSettingsWindow : public QWidget, public IPasswordSettingsWindow {
    Q_OBJECT

public:
    explicit PasswordSettingsWindow(QWidget *parent = nullptr);
    ~PasswordSettingsWindow();
    void setPasswordSettingsWindowHandler(ptr_to_passwordsettings_handler set_handler_ptr) override;
    void showErrorMessage() override;
    void createErrorMessage(const Error& error_message) override;
    std::string getOldPassword() override;
    std::string getNewPassword() override;
    std::string getRepeatPassword() override;

public slots:
    void sendSetting();
    void returnToMain();

private:
    Ui::PasswordSettingsWindow *ui;
    ptr_to_passwordsettings_handler settings_handler_ptr;

    std::string old_password;
    std::string new_password;
    
    QPushButton* btn_enter;
    QPushButton* btn_return;
    QString* error_message_;
    QString* error_type_;
    QErrorMessage* errorMes;
};

#endif // PASSWORDSETTINGSWINDOW_H