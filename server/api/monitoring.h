#ifndef MONITORING_H
#define MONITORING_H

namespace network {
    // IMonitor: Asenkron performans izleme arayüzü; üretimde Observer veya Reactor pattern uygulanabilir.
    class IMonitor {
    public:
        virtual ~IMonitor() = default;
        virtual void startMonitoring() = 0;
    };

    IMonitor* createPerformanceMonitor();
}

#endif // MONITORING_H
