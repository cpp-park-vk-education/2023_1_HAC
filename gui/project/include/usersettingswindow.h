#ifndef USERSETTINGSWINDOW_H
#define USERSETTINGSWINDOW_H

#include <QWidget>
#include <QErrorMessage>
#include <QDebug>

#include "../include/usersettingswindow_interface.h"

namespace Ui {
class UserSettingsWindow;
}

class UserSettingsWindow : public QWidget, public IUserSettingsWindow {
    Q_OBJECT

public:
    explicit UserSettingsWindow(QWidget *parent = nullptr);
    ~UserSettingsWindow();
    void setUserSettingsWindowHandler(
            ptr_to_settings_handler set_handler_ptr) override;
    void showErrorMessage() override;
    void createErrorMessage(const Error& error_message) override;
    std::string getOldPassword() override;
    std::string getNewPassword() override;
    std::string getRepeatPassword() override;

public slots:
    void sendSetting();
    void returnToMain();

private:
    Ui::UserSettingsWindow *ui;
    ptr_to_settings_handler settings_handler_ptr;

    std::string old_password;
    std::string new_password;
    
    QPushButton* btn_enter;
    QPushButton* btn_return;
    QString* error_message_;
    QString* error_type_;
    QErrorMessage* errorMes;
};

#endif // USERSETTINGSWINDOW_H
