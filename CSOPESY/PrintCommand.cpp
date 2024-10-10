#include "PrintCommand.h"
#include <iostream>
#include <fstream>
#include <filesystem>




PrintCommand::PrintCommand(int pid, std::string toPrint)
    : ICommand(pid, ICommand::PRINT), toPrint(toPrint) {}

void PrintCommand::execute() {
    std::string filename = "process_" + std::to_string(this->pid) + ".txt";
    bool fileExists = std::filesystem::exists(filename);
    std::ofstream out(filename, std::ios::app);

    if (!out.is_open()) {
        std::cerr << "Error: Could not open the file " << filename << " for writing." << std::endl;
        return;
    }

    if (!fileExists) {
        std::cout << "File created: " << filename << std::endl;
    }

    out << toPrint << std::endl;

    out.close();
}