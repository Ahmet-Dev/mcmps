#include "virtual_cpu.h"
#include <iostream>
#include <thread>

namespace core {

VirtualCPU::VirtualCPU(size_t numCores)
    : numCores(numCores)
{
    std::cout << "[VIRTUAL CPU] " << numCores << " sanal çekirdek oluşturuluyor." << std::endl;
    // Üretim ortamında, CPU planlayıcıları ve zaman dilimlendirme algoritmaları eklenebilir.
}

VirtualCPU::~VirtualCPU() {
    std::cout << "[VIRTUAL CPU] Sanal CPU kapatılıyor." << std::endl;
}

void VirtualCPU::executeTask(const std::function<void()> &task) {
    task();
}

void VirtualCPU::schedule(const std::function<void()> &task) {
    std::thread([task]() { task(); }).detach();
}

size_t VirtualCPU::getCoreCount() const {
    return numCores;
}

} // namespace core
