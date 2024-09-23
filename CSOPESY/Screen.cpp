#include "Screen.h"

// Default constructor
Screen::Screen() : id("default_id"), process(nullptr), timeCreated("00:00:00") {}

// Constructor
Screen::Screen(const string& screenId, Process* processPtr, const string& creationTime)
    : id(screenId), process(processPtr), timeCreated(creationTime) {}

// Method to display info about the process and screen
void Screen::drawScreen() {
    if (process) {
        cout << "\n--- Screen for Process: " << process->name << " ---\n";
        cout << "Instruction: " << process->currentLine << " / " << process->totalLines << endl;
        cout << "Screen Created: " << timeCreated << endl;
        cout << "-------------------------------------\n";
    }
    else {
        cout << "No valid process associated with this screen.\n";
    }
}