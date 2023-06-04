#ifndef GUICONTROLLER_INTERFACE_H
#define GUICONTROLLER_INTERFACE_H

class IWindowManager {
public:
    virtual ~IWindowManager() = default;
    virtual void openMainWindow() = 0;
    virtual void openAuthorizationWindow() = 0;
    virtual void openRegistrationWindow() = 0;
    virtual void openUserSettingsWindow() = 0;
    virtual void openPasswordSettingsWindow() = 0;
    virtual void openEmailSettingsWindow() = 0;
    virtual std::string getUser() = 0;
    virtual void setUser(const std::string& user) = 0;
    virtual void stop_timer() = 0;
    virtual void set_start_status(const std::string& status) = 0;
};

#endif // GUICONTROLLER_INTERFACE_H
