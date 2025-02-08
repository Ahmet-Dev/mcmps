#include "monitoring.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <future>

namespace network {

class PerformanceMonitor : public IMonitor {
public:
    void startMonitoring() override {
        std::cout << "[MONITORING] Performans izleme başlatıldı (asenkron veri toplama)..." << std::endl;
        // Gerçek sistemde, asenkron olarak toplanan veriler merkezi loglama servisine gönderilir.
        for (int i = 0; i < 3; ++i) {
            // Bu veriler, CPU, GPU ve bellek kullanımını gerçek API’lerden alabilir.
            std::cout << "[MONITORING] CPU: 35%, GPU: 45%, RAM: %60" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

IMonitor* createPerformanceMonitor() {
    return new PerformanceMonitor();
}

} // namespace network
