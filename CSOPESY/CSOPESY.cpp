// CSOPESY.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Process.h"
#include "Screen.h"
#include "ConsoleManager.h"
#include "GlobalScheduler.h"

int main()
{
    ConsoleManager::initialize();
    GlobalScheduler::initialize("fcfs", 1, 0, 100);
   
    bool running = true;

    while (running) {
        ConsoleManager::getInstance()->drawConsole();
        ConsoleManager::getInstance()->process();
        running = ConsoleManager::getInstance()->isRunning();
    }

    return 0;
}
