#ifndef DISTRIBUTE_H
#define DISTRIBUTE_H

#include <string>
#include <vector>

// Task: İstemcilere gönderilecek makine kodu paketini temsil eder.
struct Task {
    int id;
    std::string machineCode; // Gerçek sistemde binary kod paketi (şifreli, imzalı) olabilir.
};

// IDistributor: Dağıtım stratejileri için arayüz (Strategy Pattern)
class IDistributor {
public:
    virtual ~IDistributor() = default;
    virtual void distribute(const std::vector<Task>& tasks) = 0;
};

class RoundRobinDistributor : public IDistributor {
public:
    void distribute(const std::vector<Task>& tasks) override;
};

class WorkStealingDistributor : public IDistributor {
public:
    void distribute(const std::vector<Task>& tasks) override;
};

class PerformanceBasedDistributor : public IDistributor {
public:
    void distribute(const std::vector<Task>& tasks) override;
};

#endif // DISTRIBUTE_H
