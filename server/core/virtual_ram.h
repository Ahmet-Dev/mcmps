#ifndef VIRTUAL_RAM_H
#define VIRTUAL_RAM_H

#include <cstddef>
#include <mutex>

namespace core {

class VirtualRAM {
public:
    // Sanal RAM, minimum 8GB; üretim ortamında memory pool veya custom allocator entegre edilebilir.
    explicit VirtualRAM(size_t sizeInGB);
    ~VirtualRAM();

    void* allocate(size_t bytes);
    void deallocate(void* ptr);
    size_t getSize() const;
private:
    size_t size;
    char* ramBuffer;
    mutable std::mutex ramMutex;
};

} // namespace core

#endif // VIRTUAL_RAM_H
