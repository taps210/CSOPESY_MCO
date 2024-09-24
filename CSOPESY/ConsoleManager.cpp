#include "ConsoleManager.h"
#include "ScreenHandler.h"
#include "ProcessHandler.h"
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

ConsoleManager::ConsoleManager()
{
}

void ConsoleManager::runCommand() {
    printHeader();
    while (true) {
        // Initialize process handler and screen handler
        ProcessHandler processHandler = ProcessHandler();
        ScreenHandler screenHandler = ScreenHandler(processHandler);

        cout << "Enter a command: ";

        string command;
        getline(cin, command);

        if (command == "initialize") {
            cout << "Initialize command recognized. Doing something.\n";
        }
        else if (command == "screen") {
            // Create a new process. Sample usage of screen only!
            clearScreen();
            string processName = processHandler.createProcess("process_01", 10);
            screenHandler.createScreen(processName);
        }
        else if (command == "scheduler-test") {
            cout << "scheduler-test command recognized. Doing something.\n";
        }
        else if (command == "scheduler-stop") {
            cout << "scheduler-stop command recognized. Doing something.\n";
        }
        else if (command == "report-util") {
            cout << "Report-util command recognized. Doing something.\n";
        }
        else if (command == "clear") {
            clearScreen();
            printHeader();
        }
        else if (command == "exit") {
            cout << "Exiting...\n";
        }
        else {
            cout << command + " command not recognized." << '\n';
        }      
    }
}

