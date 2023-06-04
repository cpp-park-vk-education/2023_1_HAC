#include "../include/usecase_authorizationwindow.h"

static const std::string PATH_TO_AUTH_PARMS_FILE = "../../project/auth_parm"
                                                   ".txt";

void UseCaseAuthorizationWindow::setWindowManager(
        ptr_to_iwindow_manager wind_manager_ptr) {
    window_manager_ptr = wind_manager_ptr;
}
void UseCaseAuthorizationWindow::setAuthorizationWindow(
        ptr_to_iauthorization_window auth_window_ptr) {
    authorization_window_ptr = auth_window_ptr;
}
void UseCaseAuthorizationWindow::setAuthorizationNetwork(
        ptr_to_iauthorization_network auth_network_ptr)  {
    authorization_network_ptr = auth_network_ptr;
}
ptr_to_iauthorization_window UseCaseAuthorizationWindow::getAuthorizationWindow() {
    return authorization_window_ptr;
}
void UseCaseAuthorizationWindow::authHandler(const std::string& login,
                                              const std::string& password) {
    if (login.empty() || password.empty())  {
        // Возвряжаемся в AuthorizationWindow и выводим ошибку
        Error error_message;
        error_message.type = "EmptyInput";
        error_message.message = "Login or password was not inputed!";
        authorization_window_ptr->createErrorMessage(error_message);
        authorization_window_ptr->showErrorMessage();
        return;
    } else {
        AuthInput input;
        input.login = login;
        input.password = password;
        authorization_network_ptr->getAuthorization(input);
    }
}

void UseCaseAuthorizationWindow::passToMain(const std::string& user) {
    window_manager_ptr->setUser(user);
    window_manager_ptr->openMainWindow();
}

void UseCaseAuthorizationWindow::sendError(const Error& error_message) {
    authorization_window_ptr->createErrorMessage(error_message);
    authorization_window_ptr->showErrorMessage();
}

void UseCaseAuthorizationWindow::openRegistrationWindow() {
    window_manager_ptr->openRegistrationWindow();
}

void UseCaseAuthorizationWindow::checkedCookie(const std::string& user, const
std::string& status) {
    if (status == "good") {
        window_manager_ptr->setUser(user);
    }
    window_manager_ptr->set_start_status(status);
}

void UseCaseAuthorizationWindow::setUser(const std::string& user) {
    user_ = user;
}

void UseCaseAuthorizationWindow::checkCookie() {
    std::string line;
    std::string s;

    std::ifstream in;
    std::vector<std::string> lines;
    std::stringstream ss;

    in.open(PATH_TO_AUTH_PARMS_FILE);
    if (in.is_open()) {
        std::getline(in, line);
        if (line.empty()) {
            return;
        } else {
            ss << line;
            while(getline(ss, s, '\t')) {
                lines.push_back(s);
            }
        }
        in.close();
        if (lines.size() == 2) {
        authorization_network_ptr->setCookie(lines[0], lines[1]);
        } else {
            return;
        }
    } else {
        return;
    }
}
