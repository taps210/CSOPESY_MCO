#pragma once
#include "AScheduler.h"
#include "FlatMemoryAllocator.h"
#include <queue>

class FCFSScheduler : public AScheduler {
public:
    FCFSScheduler(int numCpu, std::shared_ptr<FlatMemoryAllocator> memoryAllocator);

    void init() override;
    void execute() override;

private:
    std::shared_ptr<SchedulerWorker> findAvailableWorker();
};