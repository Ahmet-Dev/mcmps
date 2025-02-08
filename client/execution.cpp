#include "execution.h"
#include "error_handler.h"
#include "gpu_support.h"
#include <iostream>
#include <cstring>
#ifdef USE_CUDA
#include <cuda_runtime.h>
#endif
#ifdef USE_OPENCL
#include <CL/cl.h>
#include <vector>
#endif
// DirectX ve Vulkan entegrasyonu için ilgili başlıklar eklenmelidir.

extern "C" {
    void executeAssembly(const char* code);
}

#ifdef USE_CUDA
__global__ void cudaKernel(const char* code, int* output) {
    printf("CUDA kernel received code: %s\n", code);
    *output = 42;
}
#endif

#ifdef USE_OPENCL
void openclKernel(const char* code) {
    cl_int err;
    cl_uint numPlatforms = 0;
    err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    if (err != CL_SUCCESS || numPlatforms == 0) {
        logError("OpenCL platform bulunamadı.");
        return;
    }
    cl_platform_id platform;
    err = clGetPlatformIDs(1, &platform, nullptr);
    if (err != CL_SUCCESS) {
        logError("OpenCL platform ID alınamadı.");
        return;
    }
    cl_uint numDevices = 0;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices);
    if (err != CL_SUCCESS || numDevices == 0) {
        logError("OpenCL GPU cihazı bulunamadı.");
        return;
    }
    cl_device_id device;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, nullptr);
    if (err != CL_SUCCESS) {
        logError("OpenCL cihaz ID alınamadı.");
        return;
    }
    cl_context context = clCreateContext(nullptr, 1, &device, nullptr, nullptr, &err);
    if (!context || err != CL_SUCCESS) {
        logError("OpenCL context oluşturulamadı.");
        return;
    }
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
    if (!queue || err != CL_SUCCESS) {
        logError("OpenCL komut kuyruğu oluşturulamadı.");
        clReleaseContext(context);
        return;
    }
    const char* kernelSource = code;
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, nullptr, &err);
    if (!program || err != CL_SUCCESS) {
        logError("OpenCL program oluşturulamadı.");
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return;
    }
    err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        size_t logSize;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
        std::vector<char> buildLog(logSize);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, buildLog.data(), nullptr);
        logError("OpenCL program derleme hatası: " + std::string(buildLog.begin(), buildLog.end()));
        clReleaseProgram(program);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return;
    }
    cl_kernel kernel = clCreateKernel(program, "openclKernel", &err);
    if (!kernel || err != CL_SUCCESS) {
        logError("OpenCL kernel oluşturulamadı.");
        clReleaseProgram(program);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
        return;
    }
    size_t global_work_size = 1;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &global_work_size, nullptr, 0, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        logError("OpenCL kernel enqueue hatası.");
    }
    clFinish(queue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}
#endif

#ifdef USE_DIRECTX
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
void directXKernel(const char* code) {
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                   0, nullptr, 0, D3D11_SDK_VERSION, &device, &featureLevel, &context);
    if (FAILED(hr)) {
        logError("DirectX: Cihaz oluşturulamadı.");
        return;
    }
    
    ID3DBlob* csBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    hr = D3DCompile(code, strlen(code), nullptr, nullptr, nullptr, "CSMain", "cs_5_0", 0, 0, &csBlob, &errorBlob);
    if (FAILED(hr)) {
        if (errorBlob) {
            logError(std::string("DirectX: Shader derleme hatası: ") + (char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }
        device->Release();
        context->Release();
        return;
    }
    
    ID3D11ComputeShader* computeShader = nullptr;
    hr = device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, &computeShader);
    csBlob->Release();
    if (FAILED(hr)) {
        logError("DirectX: Compute shader oluşturulamadı.");
        device->Release();
        context->Release();
        return;
    }
    
    context->CSSetShader(computeShader, nullptr, 0);
    context->Dispatch(1, 1, 1);
    context->Flush();
    
    computeShader->Release();
    context->Release();
    device->Release();
}
#endif

#ifdef USE_VULKAN
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
void vulkanKernel(const char* code) {
    VkInstance instance;
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        logError("Vulkan: Instance oluşturulamadı.");
        return;
    }
    
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        logError("Vulkan: Fiziksel cihaz bulunamadı.");
        vkDestroyInstance(instance, nullptr);
        return;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    VkPhysicalDevice physicalDevice = devices[0];
    
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0; // Basit varsayım
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    VkDevice device;
    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        logError("Vulkan: Mantıksal cihaz oluşturulamadı.");
        vkDestroyInstance(instance, nullptr);
        return;
    }
    
    // Gerçek uygulamada, SPIR-V derlenmiş compute shader kodu 'code' kullanılarak shader modülü oluşturulur.
    std::cout << "[EXECUTION] Vulkan compute pipeline kurulumu ve dispatch simülasyonu." << std::endl;
    
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
}
#endif

void executeMachineCode(const std::string &machineCode) {
    try {
        std::cout << "[EXECUTION] Makine kodu çalıştırılıyor: " << machineCode << std::endl;
        if (isCUDAAvailable()) {
#ifdef USE_CUDA
            std::cout << "[EXECUTION] CUDA API kullanılıyor." << std::endl;
            int hostResult = 0;
            int* deviceResult = nullptr;
            cudaMalloc((void**)&deviceResult, sizeof(int));
            cudaKernel<<<1, 1>>>(machineCode.c_str(), deviceResult);
            cudaError_t err = cudaDeviceSynchronize();
            if (err != cudaSuccess) {
                logError(std::string("[EXECUTION] CUDA hatası: ") + cudaGetErrorString(err));
            }
            cudaMemcpy(&hostResult, deviceResult, sizeof(int), cudaMemcpyDeviceToHost);
            std::cout << "[EXECUTION] CUDA kernel sonucu: " << hostResult << std::endl;
            cudaFree(deviceResult);
#endif
        } else if (isOpenCLAvailable()) {
#ifdef USE_OPENCL
            std::cout << "[EXECUTION] OpenCL API kullanılıyor." << std::endl;
            openclKernel(machineCode.c_str());
#endif
        } else if (isDirectXAvailable()) {
#ifdef USE_DIRECTX
            std::cout << "[EXECUTION] DirectX API kullanılıyor." << std::endl;
            directXKernel(machineCode.c_str());
#endif
        } else if (isVulkanAvailable()) {
#ifdef USE_VULKAN
            std::cout << "[EXECUTION] Vulkan API kullanılıyor." << std::endl;
            vulkanKernel(machineCode.c_str());
#endif
        } else {
            std::cout << "[EXECUTION] GPU API desteği yok; CPU üzerinde assembly çalıştırılıyor." << std::endl;
            executeAssembly(machineCode.c_str());
        }
    } catch (const std::exception &e) {
        logError(std::string("[EXECUTION] Hata: ") + e.what());
    }
}
