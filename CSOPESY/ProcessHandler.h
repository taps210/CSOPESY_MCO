#pragma once
#include <string>
#include <map>
#include <iostream>
#include "Process.h"
using namespace std;

class ProcessHandler
{
private:
    map<string, Process> processes;

public:
    // Declaration of methods
    string createProcess(const string& name, int totalLines);
    Process* getProcess(const string& name);
};
