// CSOPESY.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "Process.h"
#include "ProcessHandler.h"
#include "Screen.h"
#include "ScreenHandler.h"
#include "ConsoleManager.h"

using namespace std;

int main()
{
    ConsoleManager console = ConsoleManager();
    console.runCommand();

    return 0;
}
