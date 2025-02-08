#ifndef AUTH_H
#define AUTH_H

#include <string>

// IAuthenticator: JWT token doğrulaması için soyut arayüz (Factory Pattern)
class IAuthenticator {
public:
    virtual ~IAuthenticator() = default;
    // Gelen JWT token'ının geçerliliğini kontrol eder; asenkron I/O ile entegre edilebilir.
    virtual bool verifyToken(const std::string &token) const = 0;
};

// Factory fonksiyonu: Belirtilen secret key ile bir JWT doğrulayıcısı üretir.
IAuthenticator* createJWTAuthenticator(const std::string &secretKey);

#endif // AUTH_H
