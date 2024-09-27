#include "ScreenHandler.h"
#include <chrono>

// Constructor
ScreenHandler::ScreenHandler(ProcessHandler& ph) : processHandler(ph) {}

// Create screen for a process and add to screens map
void ScreenHandler::createScreen(const string& processName) {
    Process* process = processHandler.getProcess(processName);
    if (process) {
        // Get current time
        auto now = chrono::system_clock::now();
        std::time_t now_time = chrono::system_clock::to_time_t(now);
        std::tm local_tm;
        #ifdef _WIN32
            localtime_s(&local_tm, &now_time);
        #else
            (&now_time, &local_tm);
        #endif
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%m/%d/%Y, %I:%M:%S %p");

        string creationTime = oss.str();
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