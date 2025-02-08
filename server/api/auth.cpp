#include "auth.h"
#include <iostream>
#include <stdexcept>
#include <jwt-cpp/jwt.h> // Gerçek üretimde jwt-cpp kurulmalı

class JWTAuthenticator : public IAuthenticator {
public:
    explicit JWTAuthenticator(const std::string &secret) : secretKey(secret) {}

    bool verifyToken(const std::string &token) const override {
        try {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{secretKey})
                .with_issuer("trusted_issuer"); // Üretim ortamında, issuer dinamik veya konfigürasyona bağlı olabilir.
            verifier.verify(decoded);
            std::cout << "[AUTH] Token doğrulaması başarılı." << std::endl;
            return true;
        } catch (const std::exception &e) {
            std::cerr << "[AUTH] Token doğrulama hatası: " << e.what() << std::endl;
            return false;
        }
    }
private:
    std::string secretKey;
};

IAuthenticator* createJWTAuthenticator(const std::string &secretKey) {
    return new JWTAuthenticator(secretKey);
}
