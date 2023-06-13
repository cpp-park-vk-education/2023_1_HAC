#ifndef USERSETTINGSWINDOW_H
#define USERSETTINGSWINDOW_H

#include <QWidget>
#include <QErrorMessage>
#include <QDebug>

#include "usersettingswindow_interface.h"

namespace Ui {
class UserSettingsWindow;
}

class UserSettingsWindow : public QWidget, public IUserSettingsWindow {
    Q_OBJECT

public:
    explicit UserSettingsWindow(QWidget *parent = nullptr);
    ~UserSettingsWindow();
    
    void setUserSettingsWindowHandler(ptr_to_settings_handler set_handler_ptr) override;

public slots:
    void passwordChange();
    void emailChange();
    void returnToMain();
    void startExiting();

private:
    Ui::UserSettingsWindow *ui;
    ptr_to_settings_handler settings_handler_ptr;
    
    QPushButton* btn_password;
    QPushButton* btn_email;
    QPushButton* btn_return;
    QPushButton* btn_exit;
};

#endif // USERSETTINGSWINDOW_H
