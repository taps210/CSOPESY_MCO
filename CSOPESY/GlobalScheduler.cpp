#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include <memory>

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;
GlobalScheduler::GlobalScheduler() {
    scheduler = std::make_shared<FCFSScheduler>();
    this->start();
    scheduler->start();
}

void GlobalScheduler::initialize()
{
    if (sharedInstance == nullptr) {
        sharedInstance = new GlobalScheduler();
    }
}

void GlobalScheduler::destroy()
{
    delete sharedInstance;
    sharedInstance = nullptr;
}

GlobalScheduler* GlobalScheduler::getInstance() {
    return sharedInstance;
}

void GlobalScheduler::tick() const
{
    this->scheduler->execute();
}

std::shared_ptr<Process> GlobalScheduler::createUniqueProcess(std::string processName) {
    if (processes.find(processName) != processes.end()) {
        return nullptr;
    }

    static int nextPid = 1;
    std::shared_ptr<Process> newProcess = std::make_shared<Process>(nextPid++, processName);

    processes[processName] = newProcess;

    if (scheduler) {
        scheduler->addProcess(newProcess);
    }

    return newProcess;
}

std::shared_ptr<Process> GlobalScheduler::findProcess(std::string processName) {
    auto it = processes.find(processName);
    if (it != processes.end()) {
        return it->second;
    }
    return nullptr;
}

void GlobalScheduler::run() {
    this->scheduler->execute();
}