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


GUIController::GUIController(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUIController)
{
    ui->setupUi(this);
    auth_window = new AuthorizationWindow(this);
    setCentralWidget(auth_window);
}

GUIController::~GUIController()
{
    delete ui;
    delete auth_window;
    delete main_window;
    delete reg_window;
    delete user_settings_window;
}

