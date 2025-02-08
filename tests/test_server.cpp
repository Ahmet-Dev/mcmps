#include <iostream>
#include <cassert>
#include "auth.h"
#include "distribute.h"
#include "virtual_cpu.h"
#include "virtual_ram.h"
#include "virtual_gpu.h"
#include "error_handler.h"

int main() {
    std::cout << "[TEST SERVER] Başlatılıyor..." << std::endl;
    IAuthenticator* auth = createJWTAuthenticator("server_secret_key");
    std::string token = jwt::create()
                        .set_issuer("trusted_issuer")
                        .sign(jwt::algorithm::hs256{"server_secret_key"});
    assert(auth->verifyToken(token));
    delete auth;
    std::cout << "[TEST SERVER] Authentication testi başarılı." << std::endl;
    
    std::vector<Task> tasks;
    for (int i = 0; i < 5; i++) {
        tasks.push_back({i, "MOV EAX, " + std::to_string(i)});
    }
    RoundRobinDistributor rr;
    rr.distribute(tasks);
    std::cout << "[TEST SERVER] Dağıtım testi başarılı." << std::endl;
    
    core::VirtualCPU vcpu(4);
    vcpu.executeTask([](){ std::cout << "[TEST SERVER] VirtualCPU testi." << std::endl; });
    core::VirtualRAM vram(8);
    void* ptr = vram.allocate(1024);
    assert(ptr != nullptr);
    vram.deallocate(ptr);
    core::VirtualGPU vgpu;
    vgpu.executeKernel("TestKernel", {});
    std::cout << "[TEST SERVER] Sanal kaynaklar testi başarılı." << std::endl;
    
    std::cout << "[TEST SERVER] Tüm testler başarılı." << std::endl;
    return 0;
}
