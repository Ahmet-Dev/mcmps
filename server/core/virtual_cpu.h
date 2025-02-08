#ifndef VIRTUAL_CPU_H
#define VIRTUAL_CPU_H

#include <functional>
#include <cstddef>

namespace core {

class VirtualCPU {
public:
    explicit VirtualCPU(size_t numCores);
    ~VirtualCPU();

    // Görevi doğrudan çalıştırır
    void executeTask(const std::function<void()> &task);
    // Asenkron planlama: Gerçek üretimde, thread havuzu ve zamanlayıcı kullanılır.
    void schedule(const std::function<void()> &task);
    size_t getCoreCount() const;
private:
    size_t numCores;
};

} // namespace core

#endif // VIRTUAL_CPU_H
