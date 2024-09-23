#pragma once
#include "Process.h"
#include <string>
#include <iostream>
using namespace std;

class Screen
{
private:
    string id;
    Process* process;
    string timeCreated;

public:
    // Default constructor
    Screen();

    // Parameterized constructor
    Screen(const string& screenId, Process* processPtr, const string& creationTime);

    // Method to display info about the process and screen
    void drawScreen();
};
