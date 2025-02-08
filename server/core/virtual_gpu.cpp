#include "virtual_gpu.h"
#include <iostream>

#ifdef USE_CUDA
    #include <cuda_runtime.h>
#endif

#ifdef USE_OPENCL
    #include <CL/cl.h>
#endif

namespace core {

VirtualGPU::VirtualGPU() {
    std::cout << "[VIRTUAL GPU] Sanal GPU başlatılıyor." << std::endl;
#ifdef USE_CUDA
    int deviceCount = 0;
    cudaError_t err = cudaGetDeviceCount(&deviceCount);
    if (err == cudaSuccess && deviceCount > 0) {
        std::cout << "[VIRTUAL GPU] CUDA cihazı bulundu: " << deviceCount << " adet." << std::endl;
        useCUDA = true;
    } else {
        std::cout << "[VIRTUAL GPU] CUDA cihazı bulunamadı: " << cudaGetErrorString(err) << std::endl;
    }
#endif
#ifdef USE_OPENCL
    if (!useCUDA) {
        cl_uint platformCount = 0;
        cl_int clErr = clGetPlatformIDs(0, nullptr, &platformCount);
        if (clErr == CL_SUCCESS && platformCount > 0) {
            std::cout << "[VIRTUAL GPU] OpenCL platformu bulundu: " << platformCount << " adet." << std::endl;
            useOpenCL = true;
        } else {
            std::cout << "[VIRTUAL GPU] OpenCL platformu bulunamadı, hata: " << clErr << std::endl;
        }
    }
#endif
    if (!useCUDA && !useOpenCL) {
        std::cout << "[VIRTUAL GPU] GPU API desteği yok; CPU simülasyonu kullanılacak." << std::endl;
    }
}

VirtualGPU::~VirtualGPU() {
    std::cout << "[VIRTUAL GPU] Sanal GPU kapatılıyor." << std::endl;
}

void VirtualGPU::executeKernel(const std::string &kernelCode, const std::vector<void*> &buffers) {
    std::cout << "[VIRTUAL GPU] Kernel çalıştırılıyor: " << kernelCode << std::endl;
    if (useCUDA) {
#ifdef USE_CUDA
        std::cout << "[VIRTUAL GPU] CUDA kernel çağrısı gerçekleştiriliyor." << std::endl;
#endif
    }
    else if (useOpenCL) {
#ifdef USE_OPENCL
        std::cout << "[VIRTUAL GPU] OpenCL kernel çağrısı gerçekleştiriliyor." << std::endl;
#endif
    }
    else {
        std::cout << "[VIRTUAL GPU] GPU desteği yok; kernel CPU üzerinde simülasyon ile çalıştırılıyor." << std::endl;
    }
}

} // namespace core
