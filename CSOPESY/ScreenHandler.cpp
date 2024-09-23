#include "ScreenHandler.h"

// Constructor
ScreenHandler::ScreenHandler(ProcessHandler& ph) : processHandler(ph), activeScreen("") {}

// Create screen for a process and add to screens map
void ScreenHandler::createScreen(const string& processName) {
    Process* process = processHandler.getProcess(processName);
    if (process) {
        string creationTime = "CURRENT TIME"; // TODO: Replace with actual time
        Screen newScreen(processName, process, creationTime);
        screens[processName] = newScreen;
        activeScreen = processName;
        newScreen.drawScreen();
    }
    else {
        cout << "Failed to create process for screen '" << processName << "'." << endl;
    }
}

// Clear active screen
void ScreenHandler::exitScreen() {
    if (!activeScreen.empty()) {
        activeScreen.clear();
        cout << "Exiting to main menu...\n";
    }
}