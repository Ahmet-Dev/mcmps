#ifndef DISTRIBUTE_H
#define DISTRIBUTE_H

#include <string>

namespace core
{
    // distributeTask: Verilen görev metnini Task Manager üzerinden işleme (submit) gönderir.
    void distributeTask(const std::string &task);
}

#endif // DISTRIBUTE_H
