#pragma once
#include "AScheduler.h"
#include <queue>

class RRScheduler : public AScheduler {
public:
    RRScheduler(int timeQuantum);

    void init() override;
    void execute() override;

private:
    std::shared_ptr<SchedulerWorker> findAvailableWorker();
    int counter = 0;
    int timeQuantum;
};