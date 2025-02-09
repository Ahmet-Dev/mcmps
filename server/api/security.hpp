#ifndef SECURITY_MANAGER_HPP
#define SECURITY_MANAGER_HPP

#include "security.h"
#include <atomic>
#include <thread>
#include <string>

class SecurityManager : public ISecurityManager
{
public:
    SecurityManager();
    virtual ~SecurityManager();
    void initialize() override;
    bool validatePacket(const std::string &packet) override;

private:
    void monitorSecurity();
    std::atomic<bool> running;
    std::thread monitorThread;
    std::string secureKey;
};

#endif // SECURITY_MANAGER_HPP
