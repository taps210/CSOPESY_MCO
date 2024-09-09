// CSOPESY.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
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
    cout << "Enter a command: \n";
    return 0;
}
