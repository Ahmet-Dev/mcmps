#include "virtual_cpu.hpp"
#include <iostream>

namespace core
{

    VirtualCPU::VirtualCPU(size_t numCores)
        : numCores(numCores), stopFlag(false), timeSlice(100)
    {
        std::cout << "[VIRTUAL CPU] " << numCores << " sanal çekirdek oluşturuluyor." << std::endl;
        for (size_t i = 0; i < numCores; ++i)
        {
            workers.emplace_back(&VirtualCPU::workerThread, this);
        }
    }

    void VirtualCPU::workerThread()
    {
        while (true)
        {
            PriorityTask currentTask;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this]()
                               { return stopFlag || !taskQueue.empty(); });
                if (stopFlag && taskQueue.empty())
                    return;
                currentTask = taskQueue.top();
                taskQueue.pop();
            }
            auto startTime = std::chrono::steady_clock::now();
            bool finished = false;
            while (!finished)
            {
                finished = currentTask.task();
                std::this_thread::yield();
                auto elapsed = std::chrono::steady_clock::now() - startTime;
                if (elapsed >= timeSlice)
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    taskQueue.push(PriorityTask{currentTask.priority, currentTask.task});
                    condition.notify_one();
                    break;
                }
            }
        }
    }

    void VirtualCPU::executeTask(const CooperativeTask &task)
    {
        while (!task())
        {
            std::this_thread::yield();
        }
    }

    void VirtualCPU::schedule(int priority, const CooperativeTask &task)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.push(PriorityTask{priority, task});
        }
        condition.notify_one();
    }

    size_t VirtualCPU::getCoreCount() const
    {
        return numCores;
    }

    VirtualCPU::~VirtualCPU()
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            stopFlag = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers)
        {
            if (worker.joinable())
                worker.join();
        }
        std::cout << "[VIRTUAL CPU] Sanal CPU kapatılıyor." << std::endl;
    }

} // namespace core
