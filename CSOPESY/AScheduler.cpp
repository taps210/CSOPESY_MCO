#include "AScheduler.h"
#include <iostream>

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo)
    : schedulingAlgo(schedulingAlgo) {
    for (int i = 0; i < workersCount; i++) {
        // Initialize worker
        auto worker = std::make_shared<SchedulerWorker>(i);
        schedulerWorkers.push_back(worker);

        // Start the worker
        worker->start();
    }
}

void AScheduler::setworkersCount(int count) {
    workersCount = count;
}

void AScheduler::addProcess(std::shared_ptr<Process> process) {
    readyQueue.push(process);
}

//std::shared_ptr<Process> AScheduler::findProcess(std::string processName) {
//    for (auto& process : processes) {
//        if (process->getName() == processName) {
//            return process;
//        }
//    }
//    return nullptr;
//}

void AScheduler::run() {
    init();
}

void AScheduler::stop() {
    running = false;
}