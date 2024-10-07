//#include "ProcessHandler.h"
//
//ProcessHandler* ProcessHandler::sharedInstance = nullptr;
//ProcessHandler* ProcessHandler::getInstance() {
//    return sharedInstance;
//}
//
//void ProcessHandler::initialize() {
//    sharedInstance = new ProcessHandler();
//}
//
//void ProcessHandler::destroy() {
//    delete sharedInstance;
//}
//
//// Create a process and add it to the map of processes
//string ProcessHandler::createProcess(const string& name, int totalLines) {
//    if (processes.find(name) == processes.end()) {
//        processes[name] = Process(name, totalLines);
//        return name;
//    }
//    else {
//        cout << "Process " << name << " already exists!\n";
//        return ""; // Return empty string if the process already exists
//    }
//}
//
//// Retrieve a process from the processes map using the process name
//Process* ProcessHandler::getProcess(const string& name) {
//    if (processes.find(name) != processes.end()) {
//        return &processes[name];
//    }
//    return nullptr;
//}
//
//bool ProcessHandler::checkProcessExists(const string& processName) {
//    if (processes.find(processName) != processes.end()) {
//        return true;
//    }
//    else {
//        return false;
//    }
//}