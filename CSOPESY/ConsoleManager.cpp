#include "ConsoleManager.h"
#include "ScreenHandler.h"
#include "ProcessHandler.h"
#include <sstream>
#include <vector>
#define WINDOWS

void printHeader() {
    cout << "  CCCC   SSSSS    OOOO    PPPPP    EEEEE   SSSSS   Y   Y\n";
    cout << " C       S       O    O   P    P   E       S        Y Y \n";
    cout << " C       SSSSS   O    O   PPPPP    EEEEE   SSSSS     Y  \n";
    cout << " C           S   O    O   P        E           S     Y   \n";
    cout << "  CCCC   SSSSS    OOOO    P        EEEEE   SSSSS     Y  \n";

    cout << "\033[32m";
    cout << "Hello, Welcome to CSOPESY commandline!\n";
    cout << "\033[33m";
    cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
    cout << "\033[0m";
}

void clearScreen() {
#ifdef WINDOWS
    system("cls");  // Clear screen on Windows
#else
    system("clear"); // Clear screen on Linux/Unix/Mac
#endif
}

ConsoleManager::ConsoleManager(){}

void ConsoleManager::runCommand() {
    printHeader();
    // Initialize process handler and screen handler
    ProcessHandler processHandler = ProcessHandler();
    ScreenHandler screenHandler = ScreenHandler(processHandler);
    while (true) {
        cout << "Enter a command: ";

        string command;
        string word;
        getline(cin, command);
        stringstream ss(command);

        vector<string> args;
        while (getline(ss, word, ' ')) {
            args.emplace_back(word);
        }

        if (mode == "menu") {
            if (args[0] == "initialize") {
                cout << "Initialize command recognized. Doing something.\n";
            }
            else if (args[0] == "screen" ) {
                if (args.size() != 3) {
                    cout << "Invalid number of arguments for command screen.\n";
                }
                else if (args[1] == "-r") {
                    if (screenHandler.checkScreenExists(args[2]) && processHandler.checkProcessExists(args[2])) {
                        clearScreen();
                        mode = "screen";
                    }
                        
                    screenHandler.getScreen(args[2]);
                    
                }
                else if (args[1] == "-s") {
                    if (!processHandler.checkProcessExists(args[2])) {
                        clearScreen();
                        mode = "screen";
                    }
                    string processName = processHandler.createProcess(args[2], 10);
                    screenHandler.createScreen(processName);
                    
                }
                else {
                    cout << "Invalid argument at index 1.\n";
                }
                
            }
            else if (args[0] == "scheduler-test") {
                cout << "scheduler-test command recognized. Doing something.\n";
            }
            else if (args[0] == "scheduler-stop") {
                cout << "scheduler-stop command recognized. Doing something.\n";
            }
            else if (args[0] == "report-util") {
                cout << "Report-util command recognized. Doing something.\n";
            }
            else if (args[0] == "clear") {
                clearScreen();
                printHeader();
            }
            else if (args[0] == "exit") {
                cout << "Exiting...\n";
            }
            else {
                cout << args[0] + " command not recognized." << '\n';
            }
        }
        else if (mode == "screen") {
            if (args[0] == "process-smi") {
                cout << "process-smi command recognized. Doing something.\n";
            }
            else if (args[0] == "exit") {
                clearScreen();
                printHeader();
                mode = "menu";
            }
            else {
                cout << args[0] + " command not recognized." << '\n';
            }
        }
            
    }
}

