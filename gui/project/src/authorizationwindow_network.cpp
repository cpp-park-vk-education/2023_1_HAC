#include "../include/authorizationwindow_network.h"

void NetworkAuthorizationWindow::setAuthorizationHandler
(ptr_to_authorization_handler auth_handler_ptr) {
    authorization_handler_ptr = auth_handler_ptr;
}

void NetworkAuthorizationWindow::setAuthorizationNetwork(
        ptr_to_inetwork net_ptr) {
    network_ptr = net_ptr;
}

void NetworkAuthorizationWindow::getAuthorization(
        const AuthInput& auth_params) {
    auth_params_ = auth_params;
    std::string auth_line = auth_params.login + "\t" + auth_params.password;
    network_ptr->setConfig("AUTHORIZATION");
    network_ptr->PostRequest(url_, auth_line,
                             [this](const Error& error_state)
                             {onGetAuthorizationResponse(error_state);});
}

void NetworkAuthorizationWindow::onGetAuthorizationResponse(
        const Error& error_state) {
    if (error_state.type == "0") {
    authorization_handler_ptr->passToMain(error_state.message);
    } else {
    authorization_handler_ptr->sendError(error_state);
    }
}

void NetworkAuthorizationWindow::setCookie(const std::string& user, const std::string& cookie_line) {
    network_ptr->setConfig("CHECKCOOKIE");
    network_ptr->setCookie(cookie_line);
    network_ptr->PostRequest(url_, user,
                             [this](const Error& error_state)
                             {onSetCookieResponse(error_state);});
};
void NetworkAuthorizationWindow::onSetCookieResponse(const Error& error_state) {
    if (error_state.type == "0") {
        authorization_handler_ptr->checkedCookie(error_state.message, "good");
    } else {
        authorization_handler_ptr->checkedCookie(error_state.message, "bad");
    }
};

void NetworkAuthorizationWindow::setUrl(const std::string& url) {
    url_ = url;
}
