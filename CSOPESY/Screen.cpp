#include <chrono>

#include "Screen.h"
#include "ConsoleManager.h"

// Default constructor
Screen::Screen() : AConsole(""), attachedProcess(nullptr), timeCreated("unknown") {

}

// Constructor
Screen::Screen(std::shared_ptr<Process> processPtr)
    : AConsole(processPtr->getName()), attachedProcess(processPtr), timeCreated(processPtr->getTimeCreated()) {
}

void Screen::onEnabled() {
    display();
    process();
}

void Screen::process() {
    while (true) {
        std::cout << "root:\\\> ";

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
            display();
        }
        else if (args[0] == "exit") {
            ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
            break;
        }
        else {
            std::cout << args[0] + " command not recognized." << '\n';
        }
    }
}

void Screen::display() {
    if (attachedProcess) {
        int currentLinesOfCode = attachedProcess->getCommandCounter();
        int totalLinesOfCode = attachedProcess->getLinesOfCode();
        cout << "\nProcess: " << attachedProcess->getName() << "\n\n";
        //cout << "Screen Created: " << timeCreated << endl;

        if (currentLinesOfCode + 1 == totalLinesOfCode) {
            cout << "Finished!\n\n";
        }   
        else {
            cout << "Current instruction line: " << std::to_string(currentLinesOfCode < totalLinesOfCode ? currentLinesOfCode : currentLinesOfCode + 1) << "\n";
            cout << "Lines of code: " << std::to_string(totalLinesOfCode) << "\n\n";
        }
    }
    else {
        cout << "No valid process associated with this screen.\n";
    }
}