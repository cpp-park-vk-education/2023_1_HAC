#ifndef EMAILSETTINGSWINDOW_H
#define EMAILSETTINGSWINDOW_H

#include <QWidget>
#include <memory>
#include <QErrorMessage>
#include <QDebug>

#include "emailsettingswindow_interface.h"

namespace Ui {
class EmailSettingsWindow;
}

class EmailSettingsWindow : public QWidget, public IEmailSettingsWindow {
    Q_OBJECT

public:
    explicit EmailSettingsWindow(QWidget *parent = nullptr);
    ~EmailSettingsWindow();
    void setEmailSettingsWindowHandler(
            ptr_to_emailsettings_handler set_handler_ptr) override;
    void showErrorMessage() override;
    void createErrorMessage(const Error& error_message) override;
    std::string getNewEmail() override;
    std::string getPassword() override;

public slots:
    void sendSetting();
    void returnToMain();

private:
    Ui::EmailSettingsWindow *ui;
    ptr_to_emailsettings_handler settings_handler_ptr;

    std::string new_email;
    std::string password;

    QPushButton* btn_enter;
    QPushButton* btn_return;
    QString* error_message_;
    QString* error_type_;
    QErrorMessage* errorMes;
};

#endif // EMAILSETTINGSWINDOW_H