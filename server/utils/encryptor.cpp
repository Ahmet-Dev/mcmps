#include "encryptor.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#include <iostream>

std::string sha256(const std::string &data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return ss.str();
}

std::string aes256Encrypt(const std::string &plaintext, const std::string &key) {
    std::cout << "[ENCRYPTOR] AES-256 şifreleme işlemi gerçekleştiriliyor." << std::endl;
    // Üretimde: OpenSSL EVP API kullanılarak şifreleme yapılır.
    return "encrypted_data_stub";
}
