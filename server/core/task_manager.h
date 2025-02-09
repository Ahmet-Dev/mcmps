#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <functional>

namespace core
{
    void startTaskManager(size_t numThreads);
    void stopTaskManager();
    void submitTask(const std::function<void()> &task);
}

#endif // TASK_MANAGER_H
