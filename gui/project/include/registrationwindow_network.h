#ifndef REGISTRATIONWINDOW_NETWORK_H
#define REGISTRATIONWINDOW_NETWORK_H

#include <string>

#include "../include/registrationwindow_network_interface.h"

class NetworkRegistrationWindow : public INetworkRegistrationWindow {
public:
    void setRegistrationHandler(ptr_to_registration_handler reg_handler_ptr)
    override;
    void setRegistrationNetwork(ptr_to_inetwork net_ptr) override;
    void getRegistration(const RegInput& reg_params) override;
    void onGetRegistrationResponse(const Error& error_state) override;
    void setUser(const std::string user) override {};
    void setUrl(const std::string url) override;
private:
    ptr_to_inetwork network_ptr;
    ptr_to_registration_handler registration_handler_ptr;
    RegInput reg_params_;
    std::string url_;
};

#endif // REGISTRATIONWINDOW_NETWORK_H
