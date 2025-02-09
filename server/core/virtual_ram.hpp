#ifndef VIRTUAL_RAM_HPP
#define VIRTUAL_RAM_HPP

#include <cstddef>
#include <mutex>

namespace core
{

    class VirtualRAM
    {
    public:
        // Sanal RAM, minimum 8GB; üretim ortamında memory pool veya custom allocator entegre edilir.
        explicit VirtualRAM(size_t sizeInGB);
        ~VirtualRAM();

        // Bellek tahsisi için basit memory pool (free list) yönetimi
        void *allocate(size_t bytes);
        void deallocate(void *ptr);

        // Toplam tahsis edilen bellek boyutunu bayt cinsinden döndürür.
        size_t getSize() const;

        // Bellek bloğunun başlangıç adresini döndürür (sadece simülasyon amaçlı).
        char *getBuffer() const { return ramBuffer; }

    private:
        size_t size;     // Toplam tahsis edilen bellek boyutu (bayt)
        char *ramBuffer; // Bellek bloğu
        mutable std::mutex ramMutex;

        struct FreeBlock
        {
            size_t size;
            FreeBlock *next;
        };

        FreeBlock *freeList;

        void mergeFreeBlocks();
        size_t align8(size_t bytes) const;
    };

} // namespace core

#endif // VIRTUAL_RAM_HPP
