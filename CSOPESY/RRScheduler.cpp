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
	//cout << memoryAllocator->visualizeMemory();

	std::shared_ptr<SchedulerWorker> worker = nullptr;
	// Preempt
	for (int i = 0; i < schedulerWorkers.size(); i++) {
		//system("cls");
		//cout << "i: " << i << endl;
		//cout << "schedulerWorkers.size()" << schedulerWorkers.size() << endl;
		//cout << "schedulerWorkers[i]->getProcess()" << schedulerWorkers[i]->getProcess() << endl;
		////cout << "schedulerWorkers[i]->getProcess()->getRemainingTime()" << schedulerWorkers[i]->getProcess()->getRemainingTime() << endl;
		//if (schedulerWorkers[i]->getProcess()) {
		//	cout << "Remaining Time: " << schedulerWorkers[i]->getProcess()->getRemainingTime() << endl;
		//}
		//system("cls");
		if (i < schedulerWorkers.size() && schedulerWorkers[i]->getProcess() && schedulerWorkers[i]->getProcess()->isFinished()) {
			std::shared_ptr<Process> process = schedulerWorkers[i]->getProcess();
			schedulerWorkers[i]->assignProcess(nullptr);
			schedulerWorkers[i]->update(false);
			memoryAllocator->deallocate(process);
			process->setMemoryPtr(nullptr);
		}
		else if (i  < schedulerWorkers.size() && schedulerWorkers[i]->getProcess() && schedulerWorkers[i]->getProcess()->getRemainingTime() < 1) {
			std::shared_ptr<Process> process = schedulerWorkers[i]->getProcess();
			readyQueue.push(schedulerWorkers[i]->getProcess());
			schedulerWorkers[i]->assignProcess(nullptr);
			schedulerWorkers[i]->update(false);
			memoryAllocator->deallocate(process);
			process->setMemoryPtr(nullptr);
		}
	}
	
	while (!readyQueue.empty() && (worker = findAvailableWorker())) {
		std::shared_ptr<Process> currentProcess = readyQueue.front();
		void* memory = memoryAllocator->allocate(currentProcess);
		//cout << memoryAllocator->visualizeMemory();
		//cout << "Memory: " << memory << endl;
		if (memory != nullptr) {
			//std::cout << "Memory: " << memory << "\n";
			//std::cout << "Has enough memory. Adding process:" << currentProcess->getName() << "\n";
			std::shared_ptr<Process> currentProcess = readyQueue.front();
			currentProcess->setRemainingTime(timeQuantum);
			readyQueue.pop();
			worker->update(true);
			worker->assignProcess(currentProcess);
			currentProcess->setMemoryPtr(memory);
		}
		else {
			//std::cout << "Insufficient memory for process \n";
			break;
		}
	}

	//void RRScheduler::printBackingStore() {
	//	std::cout << "Backing Store: \n";
	//	if (backingStore.empty()) {
	//		std::cout << "Empty\n";
	//	}
	//	else {
	//		std::queue<std::shared_ptr<Process>> tempQueue = backingStore;
	//		while (!tempQueue.empty()) {
	//			auto process = tempQueue.front();
	//			tempQueue.pop();
	//			std::cout << "Process Name: " << process->getName() << ", PID: " << process->getPid() << "\n";
	//		}
	//	}
	//}

	// Execute
	for (int i = 0; i < schedulerWorkers.size(); i++) {
		if (i < schedulerWorkers.size()) {
			schedulerWorkers[i]->tick();
		}
	}
}