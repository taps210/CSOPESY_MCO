// CSOPESY.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#define WINDOWS
using namespace std;

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

int main()
{
    printHeader();

    while (true) {
        cout << "Enter a command: ";

        string command;
        getline(cin, command);
        
        if(command == "initialize"){
            cout << "Initialize command recognized. Doing something.\n";
        }
        else if(command == "screen") {
            cout << "Screen command recognized. Doing something.\n";
        }
        else if(command ==  "scheduler-test"){
            cout << "scheduler-test command recognized. Doing something.\n";
        }
        else if(command ==  "scheduler-stop"){
            cout << "scheduler-stop command recognized. Doing something.\n";
        }
        else if(command ==  "report-util"){
            cout << "Report-util command recognized. Doing something.\n";
        }
        else if(command ==  "clear"){
            clearScreen();
            printHeader();
        }
        else if(command ==  "exit"){
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << command + " command not recognized." << '\n';
        }
    }

    return 0;
}
