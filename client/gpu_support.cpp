#include "gpu_support.h"

#ifdef USE_CUDA
bool isCUDAAvailable() { return true; }
#else
bool isCUDAAvailable() { return false; }
#endif

#ifdef USE_OPENCL
bool isOpenCLAvailable() { return true; }
#else
bool isOpenCLAvailable() { return false; }
#endif

#ifdef USE_DIRECTX
bool isDirectXAvailable() { return true; }
#else
bool isDirectXAvailable() { return false; }
#endif

#ifdef USE_VULKAN
bool isVulkanAvailable() { return true; }
#else
bool isVulkanAvailable() { return false; }
#endif
