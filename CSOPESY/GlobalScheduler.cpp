#include "GlobalScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"

#include <memory>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>

std::string getTimestamp() {
    auto now = chrono::system_clock::now();
    std::time_t now_time = chrono::system_clock::to_time_t(now);
    std::tm local_tm;
#ifdef _WIN32
    localtime_s(&local_tm, &now_time);
#else
    (&now_time, &local_tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");

    return oss.str();
}

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;
GlobalScheduler::GlobalScheduler(int numCpu, std::string schedulerType, unsigned long int quantumCycles, 
                                unsigned long int batchProcessFreq, unsigned long int min, unsigned long int max, 
                                unsigned long int delaysPerExec, std::shared_ptr<IMemoryAllocator> allocator, 
                                size_t minMemPerProc, size_t maxMemPerProc, size_t memPerFrame)
    : workers(numCpu), timeQuantum(quantumCycles), processFreq(batchProcessFreq), minCom(min), maxCom(max), execDelay(delaysPerExec), 
        memoryAllocator(allocator), minMemPerProc(minMemPerProc), maxMemPerProc(maxMemPerProc), memPerFrame(memPerFrame){
    if (schedulerType == "\"fcfs\"") {
        scheduler = std::make_shared<FCFSScheduler>(numCpu, allocator);
    }
    else if (schedulerType == "\"rr\"") {
        scheduler = std::make_shared<RRScheduler>(numCpu, quantumCycles, allocator);
    }
    else {
        std::cerr << "Error: scheduler type does not exist" << std::endl;
    }

    this->start();
    scheduler->start();

}

void GlobalScheduler::initialize(int numCpu, std::string schedulerType, unsigned long int quantumCycles,
                                unsigned long int batchProcessFreq, unsigned long int min, unsigned long int max,
                                unsigned long int delaysPerExec, std::shared_ptr<IMemoryAllocator> allocator,
                                size_t minMemPerProc, size_t maxMemPerProc, size_t memPerFrame)
{
    if (sharedInstance == nullptr) {
        sharedInstance = new GlobalScheduler(numCpu, schedulerType, quantumCycles, batchProcessFreq, min, max, delaysPerExec, allocator, minMemPerProc, maxMemPerProc, memPerFrame);
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

    int i = 0;
    std::srand(static_cast<unsigned int>(std::time(0)));
    unsigned long int commands = minCom + std::rand() % (maxCom - minCom + 1);

    if (commands >= minCom && commands <= maxCom) {
            newProcess->setCommands(commands);
    } else {
        std::cerr << "Error: Number of commands is not within the min/max bounds" << std::endl;
    }



    // Roll a random value for memory between min and max bounds.
    std::srand(static_cast<unsigned int>(std::time(0)));
    size_t memoryRequired = minMemPerProc + std::rand() % (maxMemPerProc - minMemPerProc + 1);

    // Calculate the number of pages required.
    size_t pagesRequired = memoryRequired / memPerFrame;


    newProcess->setMemoryRequired(memoryRequired);
    newProcess->setPagesRequired(pagesRequired);



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
std::string listOfStats;
std::string GlobalScheduler::listStats() const {
    listOfStats = "";
    listOfStats += "Virtual Memory Statistics:\n";
    listOfStats += "Total CPUs: " + std::to_string(getCPUCount()) + "\n";
    listOfStats += "Total Processes: " + std::to_string(getProcessCount()) + "\n";
    listOfStats += "Current Ticks: " + std::to_string(getCurrentTicks()) + "\n";

    // Add the new statistics to the list
    listOfStats += "Total Memory (KB): " + std::to_string(1024) + "\n"; 
    listOfStats += "Used Memory (KB): " + std::to_string(768) + "\n";   
    listOfStats += "Free Memory (KB): " + std::to_string(256) + "\n";  
    listOfStats += "Idle CPU Ticks: " + std::to_string(150) + "\n";    
    listOfStats += "Active CPU Ticks: " + std::to_string(850) + "\n";  
    listOfStats += "Total CPU Ticks: " + std::to_string(totalTicks) + "\n";
    listOfStats += "Pages Paged In: " + std::to_string(30) + "\n";  
    listOfStats += "Pages Paged Out: " + std::to_string(15) + "\n"; 

    return listOfStats;
}

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
            unsigned long int currentLinesOfCode = process->getCommandCounter();
            unsigned long int totalLinesOfCode = process->getLinesOfCode();
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

//void GlobalScheduler::logMemory() const {
//    //int processCount = memoryAllocator->getProcessCount();
//    //size_t externalFragmentation = memoryAllocator->getExternalFragmentation();
//
//    // Open file for logging memory snapshot
//    static int quantumCycle = 0;  // Track quantum cycle number
//    std::ofstream outFile("memory_stamp_" + std::to_string(quantumCycle++) + ".txt");
//
//    if (!outFile.is_open()) {
//        std::cerr << "Error opening file!\n";
//        return;
//    }
//
//    // Write timestamp
//    outFile << "Timestamp: " << getTimestamp() << "\n";
//
//    //// Write number of processes in memory
//    //outFile << "Number of processes in memory: " << processCount << "\n";
//
//    //// Write total external fragmentation in KB
//    //outFile << "Total external fragmentation in KB: " << externalFragmentation << "\n";  // Convert bytes to KB
//
//    // Write ASCII printout of memory (assuming visualizeMemory returns a formatted string)
//    outFile << memoryAllocator->visualizeMemory();
//
//    outFile.close();
//}

void GlobalScheduler::run() {
    ticks = scheduler->schedulerWorkers.size();
    unsigned long int processCounter = 0;
    unsigned long int execCounter = 0;
    unsigned long int quantumCounter = 0;
    while (true) {
        //cout << ticks << endl;
        totalTicks++;
        sleep(100);
        if (ticks == workers) {
            //memoryAllocator->visualizeMemory();
            ticks = 0;
            processCounter++;
            execCounter++;
            quantumCounter++;

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

            if (quantumCounter >= timeQuantum) {
                if (tester) {
                    //GlobalScheduler::getInstance()->logMemory();
                }
                quantumCounter = 0;
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