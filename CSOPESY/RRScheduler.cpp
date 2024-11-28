#include "RRScheduler.h"
#include "SchedulerWorker.h"
#include "FlatMemoryAllocator.h"
#include <iostream>
#include <memory>

RRScheduler::RRScheduler(int numCpu, int timeQuantum, std::shared_ptr<IMemoryAllocator> memoryAllocator)
	: AScheduler(numCpu, SchedulingAlgorithm::ROUND_ROBIN, memoryAllocator), timeQuantum(timeQuantum) {
}

void RRScheduler::init() {
	//for (auto& process : processes) {
	//	readyQueue.push(process);
	//}
}

std::shared_ptr<SchedulerWorker> RRScheduler::findAvailableWorker() {
	for (auto& worker : schedulerWorkers) {
		if (!worker->isRunning()) {
			return worker;
		}
	}
	return nullptr;
}

void RRScheduler::execute() {
    //std::cout << memoryAllocator->visualizeMemory();

    std::shared_ptr<SchedulerWorker> worker = nullptr;

    // Handle deallocation and preemption
    for (int i = 0; i < schedulerWorkers.size(); i++) {
        if (schedulerWorkers[i]->getProcess() && schedulerWorkers[i]->getProcess()->isFinished()) {
            auto process = schedulerWorkers[i]->getProcess();
            memoryAllocator->deallocate(process);
        }
        else if (schedulerWorkers[i]->getProcess() && schedulerWorkers[i]->getProcess()->getRemainingTime() < 1) {
            auto process = schedulerWorkers[i]->getProcess();
            readyQueue.push(process);
            schedulerWorkers[i]->assignProcess(nullptr);
            schedulerWorkers[i]->update(false);
            memoryAllocator->deallocate(process);
            process->setMemoryPtr(nullptr);
        }
    }

    // Allocate from readyQueue
    while (!readyQueue.empty() && (worker = findAvailableWorker())) {
        auto currentProcess = readyQueue.front();
        void* memory = memoryAllocator->allocate(currentProcess);

        if (memory != nullptr) {
            currentProcess->setRemainingTime(timeQuantum);
            readyQueue.pop();
            worker->update(true);
            worker->assignProcess(currentProcess);
            currentProcess->setMemoryPtr(memory);
        }
        else {
            // Move process to backing store if memory is insufficient
            backingStore.push(currentProcess);
            readyQueue.pop();
            printBackingStore();
        }
    }

    // Check and allocate from backing store
    while (!backingStore.empty() && (worker = findAvailableWorker())) {
        auto currentProcess = backingStore.front();
        void* memory = memoryAllocator->allocate(currentProcess);

        if (memory != nullptr) {
            currentProcess->setRemainingTime(timeQuantum);
            backingStore.pop();
            worker->update(true);
            worker->assignProcess(currentProcess);
            currentProcess->setMemoryPtr(memory);
        }
        else {
            // Exit early since no more processes can be allocated
            break;
        }
    }

    // Execute workers
    for (int i = 0; i < schedulerWorkers.size(); i++) {
        if (i < schedulerWorkers.size()) {
            schedulerWorkers[i]->tick();
        }
    }
}

void RRScheduler::printBackingStore() {
    std::cout << "Backing Store: \n";
    if (backingStore.empty()) {
        std::cout << "Empty\n";
    }
    else {
        std::queue<std::shared_ptr<Process>> tempQueue = backingStore;
        while (!tempQueue.empty()) {
            auto process = tempQueue.front();
            tempQueue.pop();
            std::cout << "Process Name: " << process->getName() << ", PID: " << process->getPid() << "\n";
        }
    }
}