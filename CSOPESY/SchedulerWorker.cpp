#include "SchedulerWorker.h"
#include "GlobalScheduler.h"

void SchedulerWorker::update(bool isRunning) {
    this->running = running;
}

void SchedulerWorker::assignProcess(std::shared_ptr<Process> process) {
    this->currentProcess = process;
    running = true;
}

void SchedulerWorker::run() {
    while (this->running) {
        if (currentProcess) {
            currentProcess->setState(currentProcess->RUNNING);

            while (!currentProcess->isFinished()) {
                currentProcess->executeCurrentCommand();
                currentProcess->moveToNextLine();
            }

            currentProcess = nullptr;
            running = false;
        }
    }
}

bool SchedulerWorker::isRunning() {
    return running;
}