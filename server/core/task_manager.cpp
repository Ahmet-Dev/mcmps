#include "task_manager.h"
#include "error_handler.h"
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <immintrin.h>

namespace core {

class TaskManagerImpl {
public:
    explicit TaskManagerImpl(size_t numThreads)
        : stopFlag(false)
    {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(&TaskManagerImpl::workerThread, this);
        }
        std::cout << "[TASK MANAGER] " << numThreads << " iş parçacığı başlatıldı." << std::endl;
    }

    ~TaskManagerImpl() {
        stop();
        for (auto &worker : workers) {
            if (worker.joinable())
                worker.join();
        }
        std::cout << "[TASK MANAGER] Tüm iş parçacıkları kapatıldı." << std::endl;
    }

    void submitTask(const std::function<void()> &task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(task);
        }
        condition.notify_one();
    }

    void workerThread() {
        while (!stopFlag.load(std::memory_order_acquire)) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this]() {
                    return stopFlag.load(std::memory_order_acquire) || !tasks.empty();
                });
                if (stopFlag.load(std::memory_order_acquire) && tasks.empty())
                    return;
                task = tasks.front();
                tasks.pop();
            }
            _mm_pause();
            try {
                task();
            } catch (const std::exception &e) {
                logError(std::string("Görev çalıştırma hatası: ") + e.what());
            }
        }
    }

    void stop() {
        stopFlag.store(true, std::memory_order_release);
        condition.notify_all();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic_bool stopFlag;
};

static TaskManagerImpl* g_taskManager = nullptr;

void startTaskManager(size_t numThreads) {
    if (!g_taskManager) {
        g_taskManager = new TaskManagerImpl(numThreads);
    }
}

void stopTaskManager() {
    if (g_taskManager) {
        g_taskManager->stop();
        delete g_taskManager;
        g_taskManager = nullptr;
    }
}

void submitTask(const std::function<void()> &task) {
    if (g_taskManager)
        g_taskManager->submitTask(task);
}

} // namespace core
