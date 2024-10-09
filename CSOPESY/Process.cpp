#include <iostream>

#include "Process.h"
#include "PrintCommand.h"

// Constructor
Process::Process(int pid, string name)
    : pid(pid), name(name), commandCounter(0), currentState(READY) {}

// Adds a new command to the process
void Process::addCommand(ICommand::CommandType commandType) {
    std::shared_ptr<ICommand> newCommand;

    // Use the concrete subclasses based on the command type
    switch (commandType) {
    case ICommand::PRINT:
        newCommand = std::make_shared<PrintCommand>(pid, "Hello, world!");
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
