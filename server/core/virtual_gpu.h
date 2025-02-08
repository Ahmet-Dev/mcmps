#ifndef VIRTUAL_GPU_H
#define VIRTUAL_GPU_H

#include <string>
#include <vector>

namespace core {

class VirtualGPU {
public:
    VirtualGPU();
    ~VirtualGPU();

    // Gerçek sistemde, DirectX, CUDA, OpenCL veya Vulkan API entegrasyonu yapılacak.
    void executeKernel(const std::string &kernelCode, const std::vector<void*> &buffers);
private:
    bool useCUDA = false;
    bool useOpenCL = false;
};

} // namespace core

#endif // VIRTUAL_GPU_H
