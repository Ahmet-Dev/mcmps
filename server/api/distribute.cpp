#include "distribute.h"
#include <iostream>
#include <mutex>
#include <thread>

static std::mutex coutMutex;

void RoundRobinDistributor::distribute(const std::vector<Task>& tasks) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[DISTRIBUTE] Round Robin yöntemiyle dağıtım yapılıyor." << std::endl;
    int clientIndex = 0;
    for (const auto &task : tasks) {
        // Üretim ortamında: Makine kodu paketleri şifrelenip, imzalandıktan sonra asenkron olarak gönderilir.
        std::cout << "Görev " << task.id << " istemci " << clientIndex 
                  << "'a gönderiliyor. Kod: " << task.machineCode << std::endl;
        clientIndex = (clientIndex + 1) % 10;
    }
}

void WorkStealingDistributor::distribute(const std::vector<Task>& tasks) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[DISTRIBUTE] Work Stealing yöntemiyle dağıtım yapılıyor." << std::endl;
    for (const auto &task : tasks) {
        std::cout << "Görev " << task.id << " yerel kuyruğa ekleniyor; gerekirse diğer istemciden çalınacak." << std::endl;
    }
}

void PerformanceBasedDistributor::distribute(const std::vector<Task>& tasks) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << "[DISTRIBUTE] Performance-Based Allocation yöntemiyle dağıtım yapılıyor." << std::endl;
    for (const auto &task : tasks) {
        std::cout << "Görev " << task.id << " performans ölçümüne göre seçilen gruba gönderiliyor." << std::endl;
    }
}
