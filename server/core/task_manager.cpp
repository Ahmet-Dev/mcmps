#include "task_manager.h"
#include "error_handler.h"
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <iostream>

namespace core
{

    class TaskManagerImpl
    {
    public:
        TaskManagerImpl(size_t numThreads) : stopFlag(false)
        {
            for (size_t i = 0; i < numThreads; ++i)
            {
                workers.emplace_back(&TaskManagerImpl::workerThread, this);
            }
            std::cout << "[TASK MANAGER] " << numThreads << " iş parçacığı başlatıldı." << std::endl;
        }

        ~TaskManagerImpl()
        {
            stop();
            for (auto &worker : workers)
            {
                if (worker.joinable())
                    worker.join();
            }
            std::cout << "[TASK MANAGER] Tüm iş parçacıkları kapatıldı." << std::endl;
        }

        void submit(const std::function<void()> &task)
        {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                tasks.push(task);
            }
            condition.notify_one();
        }

        void stop()
        {
            stopFlag.store(true);
            condition.notify_all();
        }

    private:
        void workerThread()
        {
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this]
                                   { return stopFlag.load() || !tasks.empty(); });
                    if (stopFlag.load() && tasks.empty())
                        return;
                    task = tasks.front();
                    tasks.pop();
                }
                try
                {
                    task();
                }
                catch (const std::exception &e)
                {
                    logError(std::string("[TASK MANAGER] Görev çalıştırma hatası: ") + e.what());
                }
            }
        }

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        std::atomic<bool> stopFlag;
    };

    static TaskManagerImpl *g_taskManager = nullptr;

    void startTaskManager(size_t numThreads)
    {
        if (!g_taskManager)
        {
            g_taskManager = new TaskManagerImpl(numThreads);
        }
    }

    void stopTaskManager()
    {
        if (g_taskManager)
        {
            g_taskManager->stop();
            delete g_taskManager;
            g_taskManager = nullptr;
        }
    }

    void submitTask(const std::function<void()> &task)
    {
        if (g_taskManager)
            g_taskManager->submit(task);
    }

} // namespace core
