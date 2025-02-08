#include "error_handler.h"
#include <iostream>
#include <mutex>

static std::mutex logMutex;

void logError(const std::string &errorMessage) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::cerr << "[ERROR] " << errorMessage << std::endl;
}
