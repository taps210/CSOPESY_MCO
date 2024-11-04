#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;
GlobalScheduler::GlobalScheduler(int numCpu, std::string schedulerType, int quantumCycles, int batchProcessFreq, int min, int max, int delaysPerExec) {
    if (schedulerType == "\"fcfs\"") {
        scheduler = std::make_shared<FCFSScheduler>(numCpu);
    }
    else if (schedulerType == "\"rr\"") {
        scheduler = std::make_shared<RRScheduler>(numCpu, quantumCycles);
    }
    else {
        std::cerr << "Error: scheduler type does not exist" << std::endl;
    }

    workers = numCpu;
    processFreq = batchProcessFreq;
    minCom = min;
    maxCom = max;
    execDelay = delaysPerExec;


    this->start();
    scheduler->start();
}

void GlobalScheduler::initialize(int numCpu, std::string schedulerType, int quantumCycles, int batchProcessFreq, int min, int max, int delaysPerExec)
{
    if (sharedInstance == nullptr) {
        sharedInstance = new GlobalScheduler(numCpu, schedulerType, quantumCycles, batchProcessFreq, min, max, delaysPerExec);
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

    // Week 7
    int i = 0;
    std::srand(static_cast<unsigned int>(std::time(0)));
    int commands = minCom + std::rand() % (maxCom - minCom + 1);

    if (commands >= minCom && commands <= maxCom) {
        for (int i = 0; i < commands; i++) {
            newProcess->addCommand(ICommand::PRINT);
        }
    } else {
        std::cerr << "Error: Number of commands is not within the min/max bounds" << std::endl;
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
    // For debugging only
    int availableCores = scheduler->getAvailableCores();
    float utilization =  ((workers - availableCores) / workers) * 100;
    //cout << "CPU Cycles: " << std::to_string(cpuCycles) << "\n";
   

    listOfProcess = "";
    if (processes.empty()) {
        listOfProcess += "No processes found.\n";
        return listOfProcess;
    }
    listOfProcess += "CPU Utilization: " + std::to_string(static_cast<int>(utilization)) + "%\n";
    listOfProcess += "Cores used: " + std::to_string(workers - availableCores) + "\n";
    listOfProcess += "Cores available: " + std::to_string(availableCores) + "\n";
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
                "\t" + std::to_string(currentLinesOfCode < totalLinesOfCode ? currentLinesOfCode : currentLinesOfCode + 1) +
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
    ticks = scheduler->schedulerWorkers.size();
    int processCounter = 0;
    int execCounter = 0;
    while (true) {
        sleep(1);
        if (ticks == workers) {
            ticks = 0;
            processCounter++;
            execCounter++;

            if (execCounter >= execDelay) {    
                this->scheduler->execute();
                execCounter = 0;
            }

            if (processCounter >= processFreq) {
                if (tester) {
                    createProcess();
                }
                processCounter = 0;
            }
        }
    }
}

void GlobalScheduler::setTester(bool test) {
    tester = test;
}

bool GlobalScheduler::getTester() {
    return tester;
}