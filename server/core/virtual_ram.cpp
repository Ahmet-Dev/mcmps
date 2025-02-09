#include "virtual_ram.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace core
{

    size_t VirtualRAM::align8(size_t bytes) const
    {
        return (bytes + 7) & ~static_cast<size_t>(7);
    }

    VirtualRAM::VirtualRAM(size_t sizeInGB)
        : size(sizeInGB * 1024ULL * 1024ULL * 1024ULL), ramBuffer(nullptr), freeList(nullptr)
    {
        try
        {
            ramBuffer = new char[size];
        }
        catch (std::bad_alloc &)
        {
            std::cerr << "[VIRTUAL RAM] Bellek tahsisi başarısız!" << std::endl;
            throw;
        }
        // Başlangıçta, tüm tahsis edilen bellek tek bir free block olarak kullanıma hazır.
        freeList = reinterpret_cast<FreeBlock *>(ramBuffer);
        freeList->size = size;
        freeList->next = nullptr;
        std::cout << "[VIRTUAL RAM] " << sizeInGB << "GB sanal RAM ayarlandı." << std::endl;
    }

    VirtualRAM::~VirtualRAM()
    {
        delete[] ramBuffer;
        std::cout << "[VIRTUAL RAM] Sanal RAM serbest bırakılıyor." << std::endl;
    }

    void *VirtualRAM::allocate(size_t bytes)
    {
        std::lock_guard<std::mutex> lock(ramMutex);
        size_t reqSize = align8(bytes) + sizeof(FreeBlock);
        FreeBlock *prev = nullptr;
        FreeBlock *curr = freeList;
        while (curr)
        {
            if (curr->size >= reqSize)
            {
                if (curr->size >= reqSize + sizeof(FreeBlock) + 8)
                {
                    FreeBlock *newBlock = reinterpret_cast<FreeBlock *>(
                        reinterpret_cast<char *>(curr) + reqSize);
                    newBlock->size = curr->size - reqSize;
                    newBlock->next = curr->next;
                    if (prev)
                        prev->next = newBlock;
                    else
                        freeList = newBlock;
                    curr->size = reqSize;
                }
                else
                {
                    if (prev)
                        prev->next = curr->next;
                    else
                        freeList = curr->next;
                }
                return reinterpret_cast<void *>(reinterpret_cast<char *>(curr) + sizeof(FreeBlock));
            }
            prev = curr;
            curr = curr->next;
        }
        std::cerr << "[VIRTUAL RAM] Bellek tahsisi başarısız: Yeterli alan yok." << std::endl;
        return nullptr;
    }

    void VirtualRAM::deallocate(void *ptr)
    {
        if (!ptr)
            return;
        std::lock_guard<std::mutex> lock(ramMutex);
        FreeBlock *block = reinterpret_cast<FreeBlock *>(
            reinterpret_cast<char *>(ptr) - sizeof(FreeBlock));
        if (!freeList || block < freeList)
        {
            block->next = freeList;
            freeList = block;
        }
        else
        {
            FreeBlock *curr = freeList;
            while (curr->next && curr->next < block)
                curr = curr->next;
            block->next = curr->next;
            curr->next = block;
        }
        mergeFreeBlocks();
    }

    void VirtualRAM::mergeFreeBlocks()
    {
        FreeBlock *curr = freeList;
        while (curr && curr->next)
        {
            char *currEnd = reinterpret_cast<char *>(curr) + curr->size;
            if (currEnd == reinterpret_cast<char *>(curr->next))
            {
                curr->size += curr->next->size;
                curr->next = curr->next->next;
            }
            else
            {
                curr = curr->next;
            }
        }
    }

    size_t VirtualRAM::getSize() const
    {
        return size;
    }

} // namespace core
