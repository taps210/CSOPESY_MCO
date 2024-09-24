#include "ScreenHandler.h"

// Constructor
ScreenHandler::ScreenHandler(ProcessHandler& ph) : processHandler(ph) {}

// Create screen for a process and add to screens map
void ScreenHandler::createScreen(const string& processName) {
    Process* process = processHandler.getProcess(processName);
    if (process) {
        string creationTime = "CURRENT TIME"; // TODO: Replace with actual time
        Screen newScreen(processName, process, creationTime);
        screens[processName] = newScreen;
        newScreen.drawScreen();
    }
    else {
        cout << "Failed to create process for screen '" << processName << "'." << endl;
    }
}

bool ScreenHandler::checkScreenExists(const string& processName) {
    if (screens.find(processName) != screens.end()) {
        return true;
    }
    else {
        return false;
    }
}

// Display screen
void ScreenHandler:: getScreen(const string& processName) {
    if (checkScreenExists(processName)) {
        screens[processName].drawScreen();
    }
    else {
        cout << "Screens does not exist.\n";
    }
}