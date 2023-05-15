#include "../include/guicontroller.h"
#include "ui_guicontroller.h"
//#include "../include/guicontroller_interface.h"
/*
#include "../include/authorizationwindow.h"
#include "../include/mainwindow.h"
#include "../include/registrationwindow.h"
#include "../include/usersettingswindow.h"
#include "project/include/ionetwork.h"*/

#include "../include/authorizationwindow.h"
#include "../include/usecase_authorizationwindow.h"
#include "../include/authorizationwindow_network.h"

#include "../include/mainwindow.h"
#include "../include/usecase_mainwindow.h"
#include "../include/mainwindow_network.h"

#include "../include/usersettingswindow.h"
#include "../include/usecase_usersettingswindow.h"
#include "../include/usersettingswindow_network.h"

#include "../include/registrationwindow.h"
#include "../include/usecase_registrationwindow.h"
#include "../include/registrationwindow_network.h"


#include "../include/ionetwork.h"

GUIController::GUIController(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUIController)
{
    ui->setupUi(this);
    pages = new QStackedWidget(this);
    auth_window = new AuthorizationWindow(this);
    //setCentralWidget(auth_window);
    pages->addWidget(auth_window);
    authorization_handler_ptr = std::make_shared<UseCaseAuthorizationWindow>();
    auth_window->setAuthorizationWindowHandler(authorization_handler_ptr);
    authorization_handler_ptr->setAuthorizationWindow
    (std::shared_ptr<AuthorizationWindow>(auth_window));

    window_manager_ptr = std::shared_ptr<GUIController>(this); //???might be not the best idea

    //authorization_handler_ptr->setWindowManager(std::shared_ptr<GUIController>(this));
    authorization_handler_ptr->setWindowManager(window_manager_ptr);
    authorization_network_ptr = std::make_shared<NetworkAuthorizationWindow>();
    authorization_handler_ptr->setAuthorizationNetwork(authorization_network_ptr);
    authorization_network_ptr->setAuthorizationHandler(authorization_handler_ptr);
    authorization_network_ptr->setUrl("http://25.21.238.202:8081/");
    network_ptr = std::make_shared<IONetwork>();
    authorization_network_ptr->setAuthorizationNetwork(network_ptr);

    main_window = new MainWindow(this);
    pages->addWidget(main_window);
    main_handler_ptr = std::make_shared<UseCaseMainWindow>();
    main_window->setMainWindowHandler(main_handler_ptr);
    main_handler_ptr->setMainWindow(std::shared_ptr<MainWindow>(main_window));
    main_handler_ptr->setWindowManager(window_manager_ptr);
    main_network_ptr = std::make_shared<NetworkMainWindow>();
    main_network_ptr->setUrl("http://25.21.238.202:8081/");
    main_handler_ptr->setMainNetwork(main_network_ptr);
    main_network_ptr->setMainHandler(main_handler_ptr);
    main_network_ptr->setMainNetwork(network_ptr);

    user_settings_window = new UserSettingsWindow(this);
    pages->addWidget(user_settings_window);
    user_settings_handler_ptr = std::make_shared<UseCaseUserSettingsWindow>();
    user_settings_window->setUserSettingsWindowHandler(user_settings_handler_ptr);
    user_settings_handler_ptr->setUserSettingsWindow(std::shared_ptr<UserSettingsWindow>(user_settings_window));
    user_settings_handler_ptr->setWindowManager(window_manager_ptr);
    user_settings_network_ptr = std::make_shared<NetworkUserSettingsWindow>();
    user_settings_handler_ptr->setUserSettingsNetwork(user_settings_network_ptr);
    user_settings_network_ptr->setUrl("http://25.21.238.202:8081/");
    user_settings_network_ptr->setUserSettingsHandler(user_settings_handler_ptr);
    user_settings_network_ptr->setUserSettingsNetwork(network_ptr);

    reg_window = new RegistrationWindow(this);
    pages->addWidget(reg_window);
    registration_handler_ptr = std::make_shared<UseCaseRegistrationWindow>();
    reg_window->setRegistrationWindowHandler(registration_handler_ptr);
    registration_handler_ptr->setRegistrationWindow(std::shared_ptr<RegistrationWindow>(reg_window));
    registration_handler_ptr->setWindowManager(window_manager_ptr);
    registration_network_ptr = std::make_shared<NetworkRegistrationWindow>();
    registration_network_ptr->setUrl("http://25.21.238.202:8081/");
    registration_handler_ptr->setRegistrationNetwork(registration_network_ptr);
    registration_network_ptr->setRegistrationHandler(registration_handler_ptr);
    registration_network_ptr->setRegistrationNetwork(network_ptr);



    setCentralWidget(pages);
    pages->setCurrentIndex(0);
}

GUIController::~GUIController()
{
    delete ui;
    delete auth_window;
    delete main_window;
    delete reg_window;
    delete user_settings_window;
}

