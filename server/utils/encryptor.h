#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <string>

// Şifreleme modülü: SHA-256 hesaplama ve AES-256 şifreleme (Adapter Pattern ile farklı sağlayıcılar eklenebilir)
std::string sha256(const std::string &data);
std::string aes256Encrypt(const std::string &plaintext, const std::string &key);

#endif // ENCRYPTOR_H
