#ifndef USERSETTINGSWINDOW_H
#define USERSETTINGSWINDOW_H

#include <QWidget>

#include <string>
#include <memory>

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
            ptr_to_settings_handler set_handler_ptr) override {}
    void showErrorMessage() override {}
    void createErrorMessage(const Error& error_message) override {}
    std::string getOldPassword() override {return {};}
    std::string getNewPassword() override {return {};}

private:
    Ui::UserSettingsWindow *ui;
    ptr_to_settings_handler settings_handler_ptr;
};

#endif // USERSETTINGSWINDOW_H
