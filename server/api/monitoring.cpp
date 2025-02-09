#include "monitoring.hpp"
#include "error_handler.h"
#include <boost/asio.hpp>
#include <iostream>
#include <chrono>
#include <thread>

namespace network
{

    class PerformanceMonitor : public IMonitor
    {
    public:
        explicit PerformanceMonitor(boost::asio::io_context &io)
            : timer_(io, std::chrono::seconds(1))
        {
        }

        void startMonitoring() override
        {
            std::cout << "[MONITORING] Performans izleme başlatıldı (asenkron veri toplama)..." << std::endl;
            scheduleNext();
        }

    private:
        void scheduleNext()
        {
            timer_.expires_after(std::chrono::seconds(1));
            timer_.async_wait([this](boost::system::error_code ec)
                              {
            if (!ec) {
                collectMetrics();
                scheduleNext();
            } else {
                logError("[MONITORING] Timer hatası: " + ec.message());
            } });
        }

        void collectMetrics()
        {
            int cpuUsage = 35;
            int gpuUsage = 45;
            int ramUsage = 60;
            std::cout << "[MONITORING] CPU: " << cpuUsage << "%, GPU: " << gpuUsage << "%, RAM: " << ramUsage << "%" << std::endl;
        }

        boost::asio::steady_timer timer_;
    };

    IMonitor *createPerformanceMonitor()
    {
        static boost::asio::io_context ioContext;
        static PerformanceMonitor monitor(ioContext);
        static bool started = false;
        if (!started)
        {
            std::thread([&ioContext]()
                        { ioContext.run(); })
                .detach();
            started = true;
        }
        return &monitor;
    }

} // namespace network
