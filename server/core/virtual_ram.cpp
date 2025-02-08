#include "virtual_ram.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace core {

VirtualRAM::VirtualRAM(size_t sizeInGB)
    : size(sizeInGB * 1024ULL * 1024ULL * 1024ULL)
{
    std::cout << "[VIRTUAL RAM] " << sizeInGB << "GB sanal RAM ayarlanıyor." << std::endl;
    ramBuffer = new char[size];
    std::memset(ramBuffer, 0, size);
}

VirtualRAM::~VirtualRAM() {
    delete[] ramBuffer;
    std::cout << "[VIRTUAL RAM] Sanal RAM serbest bırakılıyor." << std::endl;
}

void* VirtualRAM::allocate(size_t bytes) {
    std::lock_guard<std::mutex> lock(ramMutex);
    void* ptr = std::malloc(bytes);
    if (!ptr) {
        std::cerr << "[VIRTUAL RAM] Bellek tahsisi başarısız!" << std::endl;
    }
    return ptr;
}

void VirtualRAM::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(ramMutex);
    std::free(ptr);
}

size_t VirtualRAM::getSize() const {
    return size;
}

} // namespace core
