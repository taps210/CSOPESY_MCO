#pragma once
#include "CSOPESYThread.h"
#include "Process.h"
#include <memory>

class SchedulerWorker : public CSOPESYThread {
public:
    SchedulerWorker() {};
    ~SchedulerWorker() = default;

    void update(bool running);
    void run();
    void assignProcess(std::shared_ptr<Process> process);
    bool isRunning();

private:
    std::shared_ptr<Process> currentProcess = nullptr;
    bool running = false;
};
