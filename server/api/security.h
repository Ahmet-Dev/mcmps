#ifndef SECURITY_H
#define SECURITY_H

#include <string>

// ISecurityManager: Güvenlik modülü arayüzü; TLS, AES-256, paket imza kontrolü, yeniden gönderme ve kötü niyetli istemci tespiti içerir.
class ISecurityManager {
public:
    virtual ~ISecurityManager() = default;
    virtual void initialize() = 0;
    virtual bool validatePacket(const std::string &packet) = 0;
};

ISecurityManager* createSecurityManager();

#endif // SECURITY_H
