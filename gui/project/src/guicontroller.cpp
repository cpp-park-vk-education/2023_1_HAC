#include "../include/guicontroller.h"
#include "ui_guicontroller.h"

#include "../include/authorizationwindow.h"
#include "../include/usecase_authorizationwindow.h"
#include "../include/authorizationwindow_network.h"

#include "../include/mainwindow.h"
#include "../include/usecase_mainwindow.h"
#include "../include/mainwindow_network.h"

#include "../include/settings/usersettingswindow.h"
#include "../include/settings/usecase_usersettingswindow.h"
#include "../include/settings/usersettingswindow_network.h"

#include "../include/settings/passwordsettingswindow.h"
#include "../include/settings/emailsettingswindow.h"
#include "../include/settings/usecase_passwordsettingswindow.h"
#include "../include/settings/usecase_emailsettingswindow.h"

#include "../include/registrationwindow.h"
#include "../include/usecase_registrationwindow.h"
#include "../include/registrationwindow_network.h"


#include "../include/ionetwork.h"

#include <QTimer>

GUIController::GUIController(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUIController)
{
    //url_ = "http://25.21.238.202:9988/";
    url_ = "http://62.84.127.93:9988/";

    ui->setupUi(this);
    pages = new QStackedWidget(this);

    auth_window = new AuthorizationWindow(this);
    pages->addWidget(auth_window);
    authorization_handler_ptr = std::make_shared<UseCaseAuthorizationWindow>();
    auth_window->setAuthorizationWindowHandler(authorization_handler_ptr);
    authorization_handler_ptr->setAuthorizationWindow
    (std::shared_ptr<AuthorizationWindow>(auth_window));
    window_manager_ptr = std::shared_ptr<GUIController>(this);
    authorization_handler_ptr->setWindowManager(window_manager_ptr);
    authorization_network_ptr = std::make_shared<NetworkAuthorizationWindow>();
    authorization_handler_ptr->setAuthorizationNetwork(authorization_network_ptr);
    authorization_network_ptr->setAuthorizationHandler(authorization_handler_ptr);
    authorization_network_ptr->setUrl(url_);
    network_ptr = std::make_shared<IONetwork>();
    network_ptr->setWindowManager(window_manager_ptr);
    authorization_network_ptr->setAuthorizationNetwork(network_ptr);

    main_window = new MainWindow(this);
    pages->addWidget(main_window);
    main_handler_ptr = std::make_shared<UseCaseMainWindow>();
    main_window->setMainWindowHandler(main_handler_ptr);
    main_handler_ptr->setMainWindow(std::shared_ptr<MainWindow>(main_window));
    main_handler_ptr->setWindowManager(window_manager_ptr);
    main_network_ptr = std::make_shared<NetworkMainWindow>();
    main_network_ptr->setUrl(url_);
    main_handler_ptr->setMainNetwork(main_network_ptr);
    main_network_ptr->setMainHandler(main_handler_ptr);
    main_network_ptr->setMainNetwork(network_ptr);

    // user_settings_window = new UserSettingsWindow(this);
    // pages->addWidget(user_settings_window);
    // user_settings_handler_ptr = std::make_shared<UseCaseUserSettingsWindow>();
    // user_settings_window->setUserSettingsWindowHandler(user_settings_handler_ptr);
    // user_settings_handler_ptr->setUserSettingsWindow(std::shared_ptr<UserSettingsWindow>(user_settings_window));
    // user_settings_handler_ptr->setWindowManager(window_manager_ptr);
    // user_settings_network_ptr = std::make_shared<NetworkUserSettingsWindow>();
    // user_settings_handler_ptr->setUserSettingsNetwork(user_settings_network_ptr);
    // user_settings_network_ptr->setUrl(url_);
    // user_settings_network_ptr->setUserSettingsHandler(user_settings_handler_ptr);
    // user_settings_network_ptr->setUserSettingsNetwork(network_ptr);


    user_settings_window = new UserSettingsWindow(this);
    pages->addWidget(user_settings_window); 
    user_settings_handler_ptr = std::make_shared<UseCaseUserSettingsWindow>();
    user_settings_window->setUserSettingsWindowHandler(user_settings_handler_ptr);
    user_settings_handler_ptr->setUserSettingsWindow(std::shared_ptr<UserSettingsWindow>(user_settings_window));
    user_settings_handler_ptr->setWindowManager(window_manager_ptr);
    // user_settings_handler_ptr->setUserSettingsNetwork(user_settings_network_ptr);
    
    user_settings_network_ptr = std::make_shared<NetworkUserSettingsWindow>();
    user_settings_handler_ptr->setUserSettingsNetwork(user_settings_network_ptr);
    password_settings_handler_ptr = std::make_shared<UseCasePasswordSettingsWindow>();
    email_settings_handler_ptr = std::make_shared<UseCaseEmailSettingsWindow>();

    password_settings_window = new PasswordSettingsWindow(this);
    pages->addWidget(password_settings_window);
    password_settings_window->setPasswordSettingsWindowHandler(password_settings_handler_ptr);
    password_settings_handler_ptr->setWindowManager(window_manager_ptr);
    password_settings_handler_ptr->setPasswordSettingsWindowHandler(std::shared_ptr<PasswordSettingsWindow>(password_settings_window));
    password_settings_handler_ptr->setUserSettingsNetwork(user_settings_network_ptr);

    email_settings_window = new EmailSettingsWindow(this);
    pages->addWidget(email_settings_window);
    email_settings_window->setEmailSettingsWindowHandler(email_settings_handler_ptr);
    email_settings_handler_ptr->setWindowManager(window_manager_ptr);
    email_settings_handler_ptr->setEmailSettingsWindow(std::shared_ptr<EmailSettingsWindow>(email_settings_window));
    email_settings_handler_ptr->setUserSettingsNetwork(user_settings_network_ptr);

    user_settings_network_ptr->setUrl(url_);
    //user_settings_network_ptr->setUserSettingsHandler(user_settings_handler_ptr);
    user_settings_network_ptr->setUserSettingsNetwork(network_ptr);
    user_settings_network_ptr->setUserPasswordSettingsHandler(password_settings_handler_ptr);
    user_settings_network_ptr->setUserEmailSettingsHandler(email_settings_handler_ptr);

    reg_window = new RegistrationWindow(this);
    pages->addWidget(reg_window);
    registration_handler_ptr = std::make_shared<UseCaseRegistrationWindow>();
    reg_window->setRegistrationWindowHandler(registration_handler_ptr);
    registration_handler_ptr->setRegistrationWindow(std::shared_ptr<RegistrationWindow>(reg_window));
    registration_handler_ptr->setWindowManager(window_manager_ptr);
    registration_network_ptr = std::make_shared<NetworkRegistrationWindow>();
    registration_network_ptr->setUrl(url_);
    registration_handler_ptr->setRegistrationNetwork(registration_network_ptr);
    registration_network_ptr->setRegistrationHandler(registration_handler_ptr);
    registration_network_ptr->setRegistrationNetwork(network_ptr);

    load_window = new LoadingWindow(this);
    load_window->setWindowManager(window_manager_ptr);
    pages->addWidget(load_window);

    first_time_in_main = true;

    //check needed
    //getting stocks from db
    //main_window->get_actions_data();

    setCentralWidget(pages);
    //pages->setCurrentIndex(0);
    pages->setCurrentIndex(6);
    //authorization_handler_ptr->check_cookie();
    openLoadWindow();
    //pages->setCurrentIndex(1);
}

GUIController::~GUIController()
{
    delete pages;
    delete ui;
    delete auth_window;
    delete main_window;
    delete reg_window;
    delete user_settings_window;
}

void GUIController::openAuthorizationWindow() {
    auth_window->clean_input_lines();
    pages->setCurrentIndex(0);
};

void GUIController::openRegistrationWindow() {
    reg_window->clean_input_lines();
    pages->setCurrentIndex(5);
};

void GUIController::stop_timer() {
    timer->stop();
}

void GUIController::openLoadWindow() {
    pages->setCurrentIndex(6);
    timer = new QTimer(this);
    //load_window->perform_progress();
    connect(timer,SIGNAL(timeout()), load_window, SLOT(perform_progress()));
    timer->start(10);
    authorization_handler_ptr->check_cookie();
    //load_window->perform_progress();

    //check needed
    //getting stocks from db
    //main_window->get_actions_data();

    QTimer::singleShot(2000, this, SLOT(change_windows()));
    //authorization_handler_ptr->check_cookie();
}

void GUIController::change_windows() {
    if (start_status == "good") {
        start_status = "";
        //check needed
        //getting stocks from db
        //main_window->get_actions_data();
        openMainWindow();
    } else {
        std::vector<std::string> vec_tmp;
        vec_tmp.push_back("sum");
        vec_tmp.push_back("here");
        //main_window->get_actions_data();
        openAuthorizationWindow();
        //openMainWindow();//temprorary
        //main_window->start_actions(vec_tmp);//temprorary
        start_status = "";
    }
}

