#pragma once
#include <iostream>
#include <map>
#include <string>
#include "ProcessHandler.h"
#include "Screen.h"
using namespace std;

class ScreenHandler
{
private:
    ProcessHandler& processHandler;
    map<string, Screen> screens;
    int lastAssignedId = 0;

public:
    // Constructor
    ScreenHandler(ProcessHandler& ph);

    // Create screen for a process and add to screens map
    void createScreen(const string& processName);

    void getScreen(const string& processName);

    bool checkScreenExists(const string& processName);
};