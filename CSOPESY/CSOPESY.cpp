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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
