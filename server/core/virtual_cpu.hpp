#ifndef VIRTUAL_CPU_HPP
#define VIRTUAL_CPU_HPP

#include <functional>
#include <cstddef>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using CooperativeTask = std::function<bool()>;

struct PriorityTask
{
    int priority;
    CooperativeTask task;
    bool operator<(const PriorityTask &other) const
    {
        return priority > other.priority;
    }
};

namespace core
{

    class VirtualCPU
    {
    public:
        explicit VirtualCPU(size_t numCores);
        ~VirtualCPU();
        void executeTask(const CooperativeTask &task);
        void schedule(int priority, const CooperativeTask &task);
        size_t getCoreCount() const;

    private:
        void workerThread();
        size_t numCores;
        std::vector<std::thread> workers;
        std::priority_queue<PriorityTask> taskQueue;
        std::mutex queueMutex;
        std::condition_variable condition;
        bool stopFlag;
        std::chrono::milliseconds timeSlice;
    };

} // namespace core

#endif // VIRTUAL_CPU_HPP
