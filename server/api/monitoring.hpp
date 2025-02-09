#ifndef MONITORING_HPP
#define MONITORING_HPP

namespace network
{
    class IMonitor
    {
    public:
        virtual ~IMonitor() = default;
        virtual void startMonitoring() = 0;
    };

    IMonitor *createPerformanceMonitor();
}

#endif // MONITORING_HPP
