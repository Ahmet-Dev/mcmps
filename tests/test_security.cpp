#include <iostream>
#include <cassert>
#include "security.h"

int main()
{
    std::cout << "[TEST SECURITY] Başlatılıyor..." << std::endl;
    ISecurityManager *secManager = createSecurityManager();
    secManager->initialize();
    assert(secManager->validatePacket("Test data"));
    assert(!secManager->validatePacket(""));
    std::cout << "[TEST SECURITY] Güvenlik testleri başarılı." << std::endl;
    delete secManager;
    return 0;
}
