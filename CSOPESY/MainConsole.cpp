#include "MainConsole.h"
#include <iostream>
#include "ConsoleManager.h"
#include "GlobalScheduler.h"
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#include <fstream>


void MainConsole::onEnabled() {
    display();
    process();
}

void MainConsole::process() {
    while (ConsoleManager::getInstance()->isRunning()) {
        std::cout << "Enter a command: ";

        if (tester == true) {
            GlobalScheduler::getInstance()->createProcess();
            std::cout << "scheduler-test command recognized. Doing something.\n";
        }

        std::string command;
        std::string word;
        getline(cin, command);
        std::stringstream ss(command);

        std::vector<std::string> args;
        while (getline(ss, word, ' ')) {
            args.emplace_back(word);
        }

        if (args[0] == "initialize") {
            initializeSystem();
            initialized = true;
        }
        else if (!initialized) {
            std::cout << "Please run 'initialize' first.\n";
        }
        else if (args[0] == "screen" && args[1] == "-ls") {
            //while (true) {
                //system("cls");
                //GlobalScheduler::getInstance()->listProcesses();
                //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            //}
            std::cout << GlobalScheduler::getInstance()->listProcesses();
        }
        else if (args[0] == "screen") {
            if (args.size() != 3) {
                std::cout << "Invalid number of arguments for command screen.\n";
            }
            else if (args[1] == "-r") {
                ConsoleManager::getInstance()->switchConsole(args[2]);
                break;
            }
            else if (args[1] == "-s") {
                const std::shared_ptr<Process> process = std::make_shared<Process>(1, args[2]);
                const std::shared_ptr<Screen> screen = std::make_shared<Screen>(process);

                try {
                    ConsoleManager::getInstance()->registerScreen(screen);
                    ConsoleManager::getInstance()->switchConsole(args[2]);
                    break;
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
            tester = true;
            std::cout << "scheduler-test command recognized. Doing something.\n";
        }
        else if (args[0] == "scheduler-stop") {
            tester = false;
            std::cout << "scheduler-stop command recognized. Doing something.\n";
        }
        else if (args[0] == "report-util") {
            GlobalScheduler::getInstance()->logProcess();
            std::cout << "Process log written to report-util.txt successfully." << std::endl;
        }

        else if (args[0] == "clear") {
            ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
        }
        else if (args[0] == "exit") {
            ConsoleManager::getInstance()->exitApplication();
            std::cout << "Exiting application...\n";
            break;
        }
        else {
            std::cout << args[0] + " command not recognized." << endl;
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

void MainConsole::initializeSystem() {
    std::ifstream configFile("config.txt");
    std::string param;
    int numCpu = 1;
    std::string schedulerType;
    int quantumCycles = 1;
    int batchProcessFreq = 1;
    int minIns = 1;
    int maxIns = 1;
    int delaysPerExec = 0;

    while (configFile >> param) {
        if (param == "num-cpu") {
            configFile >> numCpu;
            numCpu = std::clamp(numCpu, 1, 128);
        }
        else if (param == "scheduler") {
            configFile >> schedulerType;
        }
        else if (param == "quantum-cycles") {
            configFile >> quantumCycles;
        }
        else if (param == "batch-process-freq") {
            configFile >> batchProcessFreq;
        }
        else if (param == "min-ins") {
            configFile >> minIns;
        }
        else if (param == "max-ins") {
            configFile >> maxIns;
        }
        else if (param == "delays-per-exec") {
            configFile >> delaysPerExec;
        }
    }
}

void MainConsole::configureScheduler(int numCpu, const std::string& schedulerType, int quantumCycles, int batchProcessFreq, int minIns, int maxIns, int delaysPerExec) {
    // Set up CPUs and scheduler based on parsed configuration
    GlobalScheduler::getInstance()->setNumCpus(numCpu);

    //GlobalScheduler::getInstance()->setSchedulerAlgorithm(schedulerType);



    // Set other parameters as needed
}