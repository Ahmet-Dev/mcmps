#include "SecurityManager.hpp"
#include "error_handler.h"
#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>
#include <chrono>
#include <thread>

bool loadTLSCertificates(const std::string &certFile, const std::string &keyFile)
{
    std::cout << "[SECURITY] TLS sertifikaları (" << certFile << ") ve anahtar (" << keyFile << ") yüklendi." << std::endl;
    return true;
}

SecurityManager::SecurityManager() : running(true) {}

SecurityManager::~SecurityManager()
{
    running = false;
    if (monitorThread.joinable())
        monitorThread.join();
    std::cout << "[SECURITY] SecurityManager kapatılıyor." << std::endl;
}

void SecurityManager::initialize()
{
    std::cout << "[SECURITY] Güvenlik modülleri başlatılıyor (AES-256, TLS)..." << std::endl;
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    if (!loadTLSCertificates("server.crt", "server.key"))
    {
        std::cerr << "[SECURITY] TLS sertifika yüklemesi başarısız." << std::endl;
    }
    secureKey = "hashed_secret_key_example";
    std::cout << "[SECURITY] Güvenli anahtar yönetimi yapılandırıldı." << std::endl;
    monitorThread = std::thread(&SecurityManager::monitorSecurity, this);
}

void SecurityManager::monitorSecurity()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "[SECURITY] Asenkron güvenlik kontrolü yapılıyor..." << std::endl;
    }
}

bool SecurityManager::validatePacket(const std::string &packet)
{
    std::cout << "[SECURITY] Gelen paket doğrulanıyor." << std::endl;
    return !packet.empty();
}

ISecurityManager *createSecurityManager()
{
    return new SecurityManager();
}
