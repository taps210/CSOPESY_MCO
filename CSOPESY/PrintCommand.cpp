#include "PrintCommand.h"
#include <iostream>

PrintCommand::PrintCommand(int pid, std::string toPrint)
    : ICommand(pid, ICommand::PRINT), toPrint(toPrint) {}

void PrintCommand::execute() {
    std::cout << "PID " << this->pid << ": " << this->toPrint << "\n";
}