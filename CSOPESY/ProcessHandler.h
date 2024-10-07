#pragma once
#include <string>
#include <map>
#include <iostream>
#include "Process.h"
using namespace std;

class ProcessHandler
{
public:
    static ProcessHandler* getInstance();
    static void initialize();
    static void destroy();

    static string createProcess(const string& name, int totalLines);
    static Process* getProcess(const string& name);
    bool checkProcessExists(const string& processName);

private:
    ProcessHandler();
    ~ProcessHandler() = default;
    ProcessHandler(ProcessHandler const&) {}
    ProcessHandler& operator=(ProcessHandler const&) {}
    static ProcessHandler* sharedInstance;

    map<string, Process> processes;
};
