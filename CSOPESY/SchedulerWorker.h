#pragma once
#include "CSOPESYThread.h"
#include "Process.h"
#include <memory>

class SchedulerWorker : public CSOPESYThread {
public:
    SchedulerWorker() {};
    SchedulerWorker(int cpuCoreId) : cpuCoreId(cpuCoreId) {};
    ~SchedulerWorker() = default;

    void update(bool running);
    void tick();
    void run();
    void assignProcess(std::shared_ptr<Process> process);
    bool isRunning();

private:
    int cpuCoreId;
    std::shared_ptr<Process> currentProcess = nullptr;
    bool running = false;
    int cpuCyles = 0;
};
