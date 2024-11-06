#include <iostream>

#include "Process.h"
#include <chrono>

std::string getCurrentTimestamp() {
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

// Constructor
Process::Process(int pid, string name)
    : pid(pid), name(name), commandCounter(0), currentState(READY), timeCreated(getCurrentTimestamp()),remainingTime(5){}

// Adds a new command to the process
void Process::setCommands(unsigned long int numOfCommands) {
    // Add the new command to the command list
    commands = numOfCommands;
}

// Executes the current command and updates the process state accordingly
void Process::executeCurrentCommand() const {
    if (commandCounter < commands && currentState == RUNNING) {
        //commandList[commandCounter]->execute(cpuCoreId);
    }
}

// Moves to the next command in the list
void Process::moveToNextLine() {
    if (commandCounter < commands) {
        commandCounter++;
    }
    else {
        currentState = FINISHED;
    }
}

bool Process::isFinished() const {
    return currentState == FINISHED;
}

unsigned long int Process::getCommandCounter() const {
    return commandCounter;
}

unsigned long int Process::getLinesOfCode() const {
    return commands;
}

int Process::getPid() const {
    return pid;
}

int Process::getCpuCoreId() const {
    return cpuCoreId;
}

Process::ProcessState Process::getState() const {
    return currentState;
}

string Process::getName() const {
    return name;
}

string Process::getTimeCreated() const {
    return timeCreated;
}

void Process::setState(Process::ProcessState processState) {
    currentState = processState;
}

void Process::setCpuCoreId(int _cpuCoreId) {
    cpuCoreId = _cpuCoreId;
}

int Process::getRemainingTime() {
    return remainingTime;
}
void Process::setRemainingTime(int _remainingTime) {
    this->remainingTime = _remainingTime;
}
void Process::decrementRemainingTime() {
    this->remainingTime--;
}

size_t Process::getMemoryRequired() {
    return this->memoryRequired;
}