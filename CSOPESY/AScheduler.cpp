#include "AScheduler.h"
#include "FlatMemoryAllocator.h"
#include <iostream>

AScheduler::AScheduler(int numCpu, SchedulingAlgorithm schedulingAlgo, std::shared_ptr<FlatMemoryAllocator> memoryAllocator)
    : schedulingAlgo(schedulingAlgo), workersCount(numCpu), memoryAllocator(memoryAllocator) {
    for (int i = 0; i < numCpu; i++) {
        // Initialize worker
        auto worker = std::make_shared<SchedulerWorker>(i);
        schedulerWorkers.push_back(worker);

        // Start the worker
        worker->start();
    }
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

int AScheduler::getAvailableCores() {
    int availableCores = 0;
    for (int i = 0; i < schedulerWorkers.size(); i++) {
        if (!schedulerWorkers[i]->isRunning()) {
            availableCores++;
        }
    }

    return availableCores;
}

void AScheduler::run() {
    init();
}

void AScheduler::stop() {
    running = false;
}