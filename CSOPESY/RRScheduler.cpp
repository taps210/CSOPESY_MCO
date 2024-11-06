#include "RRScheduler.h"
#include "SchedulerWorker.h"
#include <iostream>

RRScheduler::RRScheduler(int numCpu, int timeQuantum)
	: AScheduler(numCpu, SchedulingAlgorithm::ROUND_ROBIN), timeQuantum(timeQuantum) {
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
	std::shared_ptr<SchedulerWorker> worker = nullptr;
	// Preempt
	for (int i = 0; i < 4; i++) {
		if (i  < schedulerWorkers.size() && schedulerWorkers[i]->getProcess() && schedulerWorkers[i]->getProcess()->getRemainingTime() < 1) {
			readyQueue.push(schedulerWorkers[i]->getProcess());
			schedulerWorkers[i]->assignProcess(nullptr);
			schedulerWorkers[i]->update(false);
		}
	}

	while (!readyQueue.empty() && (worker = findAvailableWorker())) {
		std::shared_ptr<Process> currentProcess = readyQueue.front();
		currentProcess->setRemainingTime(timeQuantum);
		readyQueue.pop();
		worker->update(true);
		worker->assignProcess(currentProcess);
	}

	// Execute
	for (int i = 0; i < schedulerWorkers.size(); i++) {
		if (i < schedulerWorkers.size()) {
			schedulerWorkers[i]->tick();
		}
	}
}