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
    std::shared_ptr<Process> getProcess();

private:
    int cpuCoreId;
    std::shared_ptr<Process> currentProcess = nullptr;
    bool running = false;
    int cpuCyles = 0;
};
