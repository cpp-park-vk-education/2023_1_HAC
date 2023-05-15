#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include <QMainWindow>
#include <QStackedWidget>

#include "../include/guicontroller_interface.h"

//#include "../include/authorizationwindow.h"
#include "../include/mainwindow.h"
#include "../include/registrationwindow.h"
#include "../include/usersettingswindow.h"

class AuthorizationWindow;

class IWindowManager;
using ptr_to_iwindow_manager = std::shared_ptr<IWindowManager>;

class IONetworkInterface;
using ptr_to_inetwork = std::shared_ptr<IONetworkInterface>;

class INetworkAuthorizationWindow;
using ptr_to_iauthorization_network =
        std::shared_ptr<INetworkAuthorizationWindow>;
class IHandlerAuthorizationWindow;
using ptr_to_authorization_handler =
        std::shared_ptr<IHandlerAuthorizationWindow>;

class IHandlerMainWindow;
using ptr_to_main_handler = std::shared_ptr<IHandlerMainWindow>;
class INetworkMainWindow;
using ptr_to_imain_network = std::shared_ptr<INetworkMainWindow>;

class IHandlerRegistrationWindow;
using ptr_to_registration_handler = std::shared_ptr<IHandlerRegistrationWindow>;
class INetworkRegistrationWindow;
using ptr_to_iregistration_network =
        std::shared_ptr<INetworkRegistrationWindow>;

class IHandlerUserSettingsWindow;
using ptr_to_settings_handler = std::shared_ptr<IHandlerUserSettingsWindow>;
class INetworkUserSettingsWindow;
using ptr_to_isettings_network = std::shared_ptr<INetworkUserSettingsWindow>;

QT_BEGIN_NAMESPACE
namespace Ui { class GUIController; }
QT_END_NAMESPACE

class GUIController : public QMainWindow, public IWindowManager {
    Q_OBJECT

public:
    GUIController(QWidget *parent = nullptr);
    ~GUIController();

    void openMainWindow() override {
        pages->setCurrentIndex(1);
    };
    void openAuthorizationWindow() override {
        pages->setCurrentIndex(0);
    };
    void openRegistrationWindow() override {
        pages->setCurrentIndex(2);
    };
    void openUserSettingsWindow() override {
        pages->setCurrentIndex(3);
    };
    virtual std::string getUser() override {
        return user_;
    };
    virtual std::string getUrl() override {};
    virtual void setUser(const std::string user) override {
        user_ = user;
    };

private:
    Ui::GUIController *ui;
    QStackedWidget* pages;

    AuthorizationWindow* auth_window;
    MainWindow* main_window;
    RegistrationWindow* reg_window;
    UserSettingsWindow* user_settings_window;

    ptr_to_inetwork network_ptr;

    ptr_to_iauthorization_network authorization_network_ptr;
    ptr_to_authorization_handler authorization_handler_ptr;

    ptr_to_imain_network main_network_ptr;
    ptr_to_main_handler main_handler_ptr;

    ptr_to_iregistration_network registration_network_ptr;
    ptr_to_registration_handler registration_handler_ptr;

    ptr_to_isettings_network user_settings_network_ptr;
    ptr_to_settings_handler user_settings_handler_ptr;

    ptr_to_iwindow_manager window_manager_ptr;

    std::string user_;
    std::string url_;

    //созданние usecase and network_inner as well
};
#endif // GUICONTROLLER_H
