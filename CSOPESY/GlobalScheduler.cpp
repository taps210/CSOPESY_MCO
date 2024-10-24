#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;
GlobalScheduler::GlobalScheduler(std::string schedulerType, int quantumCycles) {
    if (schedulerType == "fcfs") {
        scheduler = std::make_shared<FCFSScheduler>();
    }
    else if (schedulerType == "rr") {
        //TODO: set value of scheduler and pass int quantumCycles
    }
    else {
        std::cerr << "Error: scheduler type does not exist" << std::endl;
    }
    this->start();
    scheduler->start();
}

void GlobalScheduler::initialize(std::string schedulerType, int quantumCycles)
{
    if (sharedInstance == nullptr) {
        sharedInstance = new GlobalScheduler(schedulerType, quantumCycles);
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

// Week 7
void GlobalScheduler::createProcess() {
    j++;
    GlobalScheduler::getInstance()->createUniqueProcess("testprocess_" + std::to_string(j));
}

std::string listOfProcess;

std::string GlobalScheduler::listProcesses() const {
    listOfProcess = "";
    if (processes.empty()) {
        listOfProcess += "No processes found.\n";
        return listOfProcess;
    }

    listOfProcess += ("------------------------------------------\n");
    listOfProcess += "Running processes:\n";

    for (const auto& p : processes) {
        auto process = p.second;
        if (!process->isFinished()) {
            int currentLinesOfCode = process->getCommandCounter();
            int totalLinesOfCode = process->getLinesOfCode();
            std::string coreIdOutput = (process->getCpuCoreId() == -1) ? "N/A" : std::to_string(process->getCpuCoreId());
            listOfProcess += process->getName() + "\t(" +
                process->getTimeCreated() + ")" +
                "\tCore: " + coreIdOutput +
                "\t" + std::to_string(currentLinesOfCode == 0 ? currentLinesOfCode : currentLinesOfCode + 1) +
                " / " + std::to_string(totalLinesOfCode) + "\n";
        }
    }

    listOfProcess += "\nFinished processes:\n";
    for (const auto& p : processes) {
        auto process = p.second;
        if (process->isFinished()) {
            int totalLinesOfCode = process->getLinesOfCode();
            listOfProcess += process->getName() + "\t(" +
                process->getTimeCreated() + ")" +
                "\tFinished\t" +
                std::to_string(totalLinesOfCode) + " / " + std::to_string(totalLinesOfCode) + "\n";
        }
    }

    listOfProcess += "------------------------------------------\n";
    
    return listOfProcess;
}

void GlobalScheduler::logProcess() const {
    std::string outputFilename = "report-util.txt";
    std::ofstream out(outputFilename, std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Error: Could not open the log file!" << std::endl;
        return;
    }

    out << listOfProcess;
    out.close();
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

void GlobalScheduler::setNumCpus(int numCpu) {
    this->scheduler->setworkersCount(numCpu);
}

void setSchedulerAlgorithm(string schedulerType) {
    //this->scheduler->
}