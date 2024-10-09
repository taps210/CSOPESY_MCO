#include "MainConsole.h"
#include <iostream>
#include "ConsoleManager.h"
#include <sstream>
#include <vector>
#include <string>

void MainConsole::onEnabled() {
    display();
    process();
}

void MainConsole::process() {
    while (true) {
        std::cout << "Enter a command: ";

        std::string command;
        std::string word;
        getline(cin, command);
        std::stringstream ss(command);

        std::vector<std::string> args;
        while (getline(ss, word, ' ')) {
            args.emplace_back(word);
        }

        if (args[0] == "initialize") {
            std::cout << "Initialize command recognized. Doing something.\n";
        }
        else if (args[0] == "screen") {
            if (args.size() != 3) {
                std::cout << "Invalid number of arguments for command screen.\n";
            }
            else if (args[1] == "-r") {
                ConsoleManager::getInstance()->switchConsole(args[2]);
            }
            else if (args[1] == "-s") {
                const std::shared_ptr<Process> process = std::make_shared<Process>(1, args[2]);
                const std::shared_ptr<Screen> screen = std::make_shared<Screen>(process);

                try {
                    ConsoleManager::getInstance()->registerScreen(screen);
                    ConsoleManager::getInstance()->switchConsole(args[2]);
                }
                catch (const std::exception& e) {
                    std::cout << e.what();
                }
            }
            else {
                std::cout << "Invalid argument at index 1.\n";
            }

        }
        else if (args[0] == "scheduler-test") {
            std::cout << "scheduler-test command recognized. Doing something.\n";
        }
        else if (args[0] == "scheduler-stop") {
            std::cout << "scheduler-stop command recognized. Doing something.\n";
        }
        else if (args[0] == "report-util") {
            std::cout << "Report-util command recognized. Doing something.\n";
        }
        else if (args[0] == "clear") {
            ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
        }
        else if (args[0] == "exit") {
            // TODO: Replace with non-brute force approach. Use ConsoleManager::exitApplication()
            exit(0);
        }
        else {
            std::cout << args[0] + " command not recognized." << '\n';
        }
    }
}

void MainConsole::display() {
    std::cout << "  CCCC   SSSSS    OOOO    PPPPP    EEEEE   SSSSS   Y   Y\n";
    std::cout << " C       S       O    O   P    P   E       S        Y Y \n";
    std::cout << " C       SSSSS   O    O   PPPPP    EEEEE   SSSSS     Y  \n";
    std::cout << " C           S   O    O   P        E           S     Y   \n";
    std::cout << "  CCCC   SSSSS    OOOO    P        EEEEE   SSSSS     Y  \n";

    std::cout << "\033[32m";
    std::cout << "Hello, Welcome to CSOPESY commandline!\n";
    std::cout << "\033[33m";
    std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
    std::cout << "\033[0m";
}