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

void GlobalScheduler::tick() {
    ticks = ticks + 1;
}

std::shared_ptr<Process> GlobalScheduler::createUniqueProcess(std::string processName) {
    if (processes.find(processName) != processes.end()) {
        return nullptr;
    }

    static int nextPid = 0;
    std::shared_ptr<Process> newProcess = std::make_shared<Process>(nextPid++, processName);

    // Week 6
    for (int i = 0; i < 100; i++) {
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
    //cout << "CPU Cycles: " << cpuCycles << " " << "Ticks: " << ticks << " " << "Workers: " << scheduler->schedulerWorkers.size() << "\n";
    if (processes.empty()) {
        std::cout << "No processes found. \n";
        return;
    }

    std::cout << "------------------------------------------\n";
    std::cout << "Running processes:\n";

    for (const auto& p : processes) {
        auto process = p.second;
        if (!process->isFinished()) {
            int currentLinesOfCode = process->getCommandCounter();
            int totalLinesOfCode = process->getLinesOfCode();
            std::string coreIdOutput = (process->getCpuCoreId() == -1) ? "N/A" : std::to_string(process->getCpuCoreId());
            std::cout << process->getName() << "\t("
                << process->getTimeCreated() << ")"
                << "\tCore: " << coreIdOutput
                << "\t" << (currentLinesOfCode == 0 ? currentLinesOfCode : currentLinesOfCode + 1)
                << " / " << totalLinesOfCode << "\n";
        }
    }

    std::cout << "\nFinished processes:\n";
    for (const auto& p : processes) {
        auto process = p.second;
        if (process->isFinished()) {
            int totalLinesOfCode = process->getLinesOfCode();
            std::cout << process->getName() << "\t("
                << process->getTimeCreated() << ")"
                << "\tFinished\t"
                << totalLinesOfCode << " / " << totalLinesOfCode << "\n";
        }
    }

    std::cout << "------------------------------------------\n";
}

std::shared_ptr<Process> GlobalScheduler::findProcess(std::string processName) {
    auto it = processes.find(processName);
    if (it != processes.end()) {
        return it->second;
    }
    return nullptr;
}

void GlobalScheduler::run() {
    ticks = scheduler->schedulerWorkers.size();
    while (true) {
        sleep(50);
        if (ticks == scheduler->schedulerWorkers.size()) {
            ticks = 0;
            this->cpuCycles++;
            this->scheduler->execute();
        }
    }
}