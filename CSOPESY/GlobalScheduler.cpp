#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include <memory>
#include <iostream>

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

    // Week 6
    for (int i = 0; i < 10; i++) {
        newProcess->addCommand(ICommand::PRINT);
    }

    processes[processName] = newProcess;

    if (scheduler) {
        scheduler->addProcess(newProcess);
    }

    return newProcess;
}

// Week 6
void GlobalScheduler::create10Processes() {
    for (int i = 0; i < 10; i++) {
        GlobalScheduler::getInstance()->createUniqueProcess("process_" + std::to_string(i));
    }
}

void GlobalScheduler::listProcesses() const {
    if (processes.empty()) {
        std::cout << "No processes found. \n";
        return;
    }

    for (const auto& p : processes) {
        auto process = p.second;

        std::string status;
        if (process->isFinished()) {
            status = "Finished";
        }

        else {
            status = "Core: " + std::to_string(process->getCpuCoreId());
        }

        int currentLinesOfCode = process->getCommandCounter();
        int totalLinesOfCode = process->getLinesOfCode();

        std::cout << "Process Name: " << process->getName()
            << " | PID: " << process->getPid()
            << " | Status: " << (process->isFinished() ? "Finished" : "Not Finished")
            << " | Core ID: " << (process->isFinished() ? "N/A" : std::to_string(process->getCpuCoreId()))
            << " | Lines of Code: " << currentLinesOfCode << "/" << totalLinesOfCode
            << "\n";
    }
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