#ifndef GUICONTROLLER_INTERFACE_H
#define GUICONTROLLER_INTERFACE_H

class IWindowManager {
public:
    virtual ~IWindowManager() = default;
    virtual void openMainWindow() = 0;
    virtual void openAuthorizationWindow() = 0;
    virtual void openRegistrationWindow() = 0;
    virtual void openUserSettingsWindow() = 0;
    virtual std::string getUser() = 0;
    virtual std::string getUrl() = 0;
    virtual void setUser(const std::string user) = 0;
};

#endif // GUICONTROLLER_INTERFACE_H
