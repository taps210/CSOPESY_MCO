// CSOPESY.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
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

int main()
{
    printHeader();

    while (true) {
        cout << "Enter a command: ";

        string command;
        cin >> command;
        
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
            cout << "\033[2J\033[1;1H";
        }
        else if(command ==  "exit"){
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << command + " command not recognized.";
        }
    }

    return 0;
}
