#include "auth.h"
#include "error_handler.h"
#include <iostream>
#include <jwt-cpp/jwt.h>

bool clientAuthenticate(const std::string &jwtToken) {
    try {
        auto decoded = jwt::decode(jwtToken);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{"client_secret"})
            .with_issuer("trusted_client");
        verifier.verify(decoded);
        std::cout << "[CLIENT AUTH] Kimlik doğrulaması başarılı." << std::endl;
        return true;
    } catch (const std::exception &e) {
        logError(std::string("[CLIENT AUTH] Hata: ") + e.what());
        return false;
    }
}
