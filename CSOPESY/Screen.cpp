#include <chrono>

#include "Screen.h"
#include "ConsoleManager.h"

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

// Default constructor
Screen::Screen() : AConsole(""), attachedProcess(nullptr), timeCreated("unknown") {

}

// Constructor
Screen::Screen(std::shared_ptr<Process> processPtr)
    : AConsole(processPtr->getName()), attachedProcess(processPtr), timeCreated(getCurrentTimestamp()) {
}

void Screen::onEnabled() {
    display();
    process();
}

void Screen::process() {
    while (true) {
        std::cout << "Enter a command: ";

        string command;
        string word;
        getline(cin, command);
        stringstream ss(command);

        vector<string> args;
        while (getline(ss, word, ' ')) {
            args.emplace_back(word);
        }

        if (args[0] == "process-smi") {
            std::cout << "process-smi command recognized. Doing something.\n";
        }
        else if (args[0] == "exit") {
            ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
        }
        else {
            std::cout << args[0] + " command not recognized." << '\n';
        }
    }
}

void Screen::display() {
    if (attachedProcess) {
        cout << "\n--- Screen for Process: " << attachedProcess->getName() << " ---\n";
        cout << "Instruction: " << attachedProcess->getCommandCounter() << " / " << attachedProcess->getLinesOfCode() << endl;
        cout << "Screen Created: " << timeCreated << endl;
        cout << "-------------------------------------\n";
    }
    else {
        cout << "No valid process associated with this screen.\n";
    }
}