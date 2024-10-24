#include "PrintCommand.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

PrintCommand::PrintCommand(int pid, std::string toPrint)
    : ICommand(pid, ICommand::PRINT), toPrint(toPrint) {}


std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
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


void PrintCommand::execute(int cpuCoreId) {
    //std::string filename = "process_" + std::to_string(this->pid) + ".txt";
    //bool fileExists = std::filesystem::exists(filename);
    //std::ofstream out(filename, std::ios::app);

    //if (!out.is_open()) {
    //    std::cerr << "Error: Could not open the file " << filename << " for writing." << std::endl;
    //    return;
    //}

    //if (!fileExists) {
    //    //std::cout << "File created: " << filename << std::endl;
    //    out << "Process name: " << "process_" << this->pid << "\n";
    //    out << "Logs:\n\n";
    //}

   

    //out << "(" << getTimestamp() << ") "
    //    << "\tCore:" << cpuCoreId << "\t\t" << toPrint << "\n";

    //out.close();
}