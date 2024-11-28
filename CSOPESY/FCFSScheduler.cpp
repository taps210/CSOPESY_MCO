#include "FCFSScheduler.h"
#include "SchedulerWorker.h"
#include "FlatMemoryAllocator.h"
#include <iostream>

FCFSScheduler::FCFSScheduler(int numCpu, std::shared_ptr<IMemoryAllocator> memoryAllocator)
	: AScheduler(numCpu, SchedulingAlgorithm::FCFS, memoryAllocator) {
}

void FCFSScheduler::init() {
	//for (auto& process : processes) {
	//	readyQueue.push(process);
	//}
}

std::shared_ptr<SchedulerWorker> FCFSScheduler::findAvailableWorker() {
	for (auto& worker : schedulerWorkers) {
		if (!worker->isRunning()) {
			return worker;
		}
	}
	return nullptr;
}

void FCFSScheduler::execute() {
	std::shared_ptr<SchedulerWorker> worker = nullptr;

	for (int i = 0; i < schedulerWorkers.size(); i++) {
		if (i < schedulerWorkers.size() && schedulerWorkers[i]->getProcess() && schedulerWorkers[i]->getProcess()->isFinished()) {
			std::shared_ptr<Process> process = schedulerWorkers[i]->getProcess();
			schedulerWorkers[i]->assignProcess(nullptr);
			schedulerWorkers[i]->update(false);
			memoryAllocator->deallocate(process);
			process->setMemoryPtr(nullptr);
		}
	}

	while (!readyQueue.empty() && (worker = findAvailableWorker())) {
		std::shared_ptr<Process> currentProcess = readyQueue.front();
		void* memory = memoryAllocator->allocate(currentProcess);

		if (memory != nullptr) {
			//std::cout << "Memory: " << memory << "\n";
			//std::cout << "Has enough memory. Adding process:" << currentProcess->getName() << "\n";
			readyQueue.pop();
			worker->update(true);
			worker->assignProcess(currentProcess);
			currentProcess->setMemoryPtr(memory);
		}
		else {
			break;
		}
	}
	for (int i = 0; i < schedulerWorkers.size(); i++) {
		schedulerWorkers[i]->tick();
	}
}