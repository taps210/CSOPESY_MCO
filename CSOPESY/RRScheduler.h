#pragma once
#include "AScheduler.h"
#include <queue>
#include "FlatMemoryAllocator.h"

class RRScheduler : public AScheduler {
public:
    RRScheduler(int numCpu, int timeQuantum);

    void init() override;
    void execute() override;

private:
    std::shared_ptr<SchedulerWorker> findAvailableWorker();
    int counter = 0;
    int timeQuantum;
    const size_t maximumMemorySize = 50;
    FlatMemoryAllocator* memoryAllocator = new FlatMemoryAllocator(maximumMemorySize);
};