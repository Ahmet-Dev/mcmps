#include "distribute.h"
#include "core/task_manager.h" // Task Manager fonksiyonlarını kullanacağız.
#include <iostream>
#include <thread>
#include <chrono>

namespace core
{

    void distributeTask(const std::string &task)
    {
        // Task Manager aracılığıyla görevi kuyruğa ekliyoruz.
        submitTask([task]()
                   {
        std::cout << "[DISTRIBUTE] Görev işleniyor: " << task << std::endl;
        // Gerçek iş yükü burada gerçekleştirilebilir.
        // Örneğin, görev işlenirken simülasyon amaçlı kısa bir gecikme ekliyoruz.
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        std::cout << "[DISTRIBUTE] Görev tamamlandı: " << task << std::endl; });
    }

} // namespace core
