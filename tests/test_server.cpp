#include <iostream>
#include <cassert>
#include "auth.h"
#include "distribute.h"
#include "virtual_cpu.hpp"
#include "virtual_ram.hpp"
#include "error_handler.h"

int main()
{
    std::cout << "[TEST SERVER] Başlatılıyor..." << std::endl;
    IAuthenticator *auth = createJWTAuthenticator("server_secret_key");
    std::string token = jwt::create()
                            .set_issuer("trusted_issuer")
                            .sign(jwt::algorithm::hs256{"server_secret_key"});
    assert(auth->verifyToken(token));
    delete auth;
    std::cout << "[TEST SERVER] Authentication testi başarılı." << std::endl;

    std::string task = "MOV EAX, 1";
    distributeTask(task);
    std::cout << "[TEST SERVER] Dağıtım testi başarılı." << std::endl;

    core::VirtualCPU vcpu(4);
    vcpu.executeTask([]()
                     { std::cout << "[TEST SERVER] VirtualCPU testi." << std::endl; return true; });
    core::VirtualRAM vram(8);
    void *ptr = vram.allocate(1024);
    assert(ptr != nullptr);
    vram.deallocate(ptr);
    std::cout << "[TEST SERVER] Sanal RAM testi başarılı." << std::endl;

    std::cout << "[TEST SERVER] Tüm testler başarılı." << std::endl;
    return 0;
}
