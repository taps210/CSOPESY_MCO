#include <iostream>

#include "Process.h"
#include "PrintCommand.h"
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
    : pid(pid), name(name), commandCounter(0), currentState(READY), timeCreated(getCurrentTimestamp()){}

// Adds a new command to the process
void Process::addCommand(ICommand::CommandType commandType) {
    std::shared_ptr<ICommand> newCommand;

    // Use the concrete subclasses based on the command type
    switch (commandType) {
    case ICommand::PRINT:
        newCommand = std::make_shared<PrintCommand>(pid, "Hello, world from " + getName() + "!");
        break;
    default:
        std::cout << "Invalid argument";
    }

    // Add the new command to the command list
    commandList.push_back(newCommand);
}

// Executes the current command and updates the process state accordingly
void Process::executeCurrentCommand() const {
    if (commandCounter < commandList.size() && currentState == RUNNING) {
        commandList[commandCounter]->execute();
    }
}

// Moves to the next command in the list
void Process::moveToNextLine() {
    if (commandCounter < commandList.size() - 1) {
        commandCounter++;
    }
    else {
        currentState = FINISHED;
    }
}

bool Process::isFinished() const {
    return currentState == FINISHED;
}

int Process::getCommandCounter() const {
    return commandCounter;
}

int Process::getLinesOfCode() const {
    return commandList.size();
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