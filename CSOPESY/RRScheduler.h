#pragma once
#include "AScheduler.h"
#include <queue>
#include "IMemoryAllocator.h"
#include "FlatMemoryAllocator.h"

class RRScheduler : public AScheduler {
public:
    RRScheduler(int numCpu, int timeQuantum, std::shared_ptr<IMemoryAllocator> memoryAllocator);

    void init() override;
    void execute() override;
    void printBackingStore();

private:
    std::shared_ptr<SchedulerWorker> findAvailableWorker();
    std::queue<std::shared_ptr<Process>> backingStore;
    int counter = 0;
    int timeQuantum;
    const size_t maximumMemorySize = 16384;
};