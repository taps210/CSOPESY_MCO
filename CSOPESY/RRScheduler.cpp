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

	if (!readyQueue.empty()) {
		std::shared_ptr<Process> currentProcess = readyQueue.front();
		void* memory = memoryAllocator->allocateBackingStore(currentProcess);
		int i = 0;
		//cout << memoryAllocator->visualizeMemory();
		//cout << "Memory: " << memory << endl;

		if (memory != nullptr) {
			//std::cout << "Memory: " << memory << "\n";
			//std::cout << "Has enough memory. Adding process:" << currentProcess->getName() << "\n";
			currentProcess->setRemainingTime(timeQuantum);
			readyQueue.pop();
			worker = findAvailableWorker();
			worker->update(true);
			worker->assignProcess(currentProcess);
			currentProcess->setMemoryPtr(memory);
		}
		else {
			//std::cout << "Insufficient memory for process \n";
			while (memory == nullptr) {
				readyQueue.push(schedulerWorkers[i]->getProcess());
				schedulerWorkers[i]->assignProcess(nullptr);
				schedulerWorkers[i]->update(false);
				i++;
				memory = memoryAllocator->allocateBackingStore(currentProcess);
				if (memory != nullptr) {
					currentProcess->setRemainingTime(timeQuantum);
					readyQueue.pop();
					worker = findAvailableWorker();
					worker->update(true);
					worker->assignProcess(currentProcess);
					currentProcess->setMemoryPtr(memory);
				}
			}
		}
	}
	
	//while (!readyQueue.empty() && (worker = findAvailableWorker())) {
	//	std::shared_ptr<Process> currentProcess = readyQueue.front();
	//	void* memory = memoryAllocator->allocateBackingStore(currentProcess);
	//	//cout << memoryAllocator->visualizeMemory();
	//	//cout << "Memory: " << memory << endl;

	//	if (memory != nullptr) {
	//		//std::cout << "Memory: " << memory << "\n";
	//		//std::cout << "Has enough memory. Adding process:" << currentProcess->getName() << "\n";
	//		currentProcess->setRemainingTime(timeQuantum);
	//		readyQueue.pop();
	//		worker->update(true);
	//		worker->assignProcess(currentProcess);
	//		currentProcess->setMemoryPtr(memory);
	//	}
	//	else {
	//		//std::cout << "Insufficient memory for process \n";
	//		while (memory == nullptr) {
	//			memory = memoryAllocator->allocateBackingStore(currentProcess);
	//			if (memory != nullptr) {
	//				currentProcess->setRemainingTime(timeQuantum);
	//				readyQueue.pop();
	//				worker->update(true);
	//				worker->assignProcess(currentProcess);
	//				currentProcess->setMemoryPtr(memory);
	//			}
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