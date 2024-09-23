#pragma once
#include <string>
using namespace std;

class Process
{
public:
    string name;
    int currentLine;
    int totalLines;

    // Default constructor
    Process();

    // Constructor
    Process(const string processName, int totalLines);
};