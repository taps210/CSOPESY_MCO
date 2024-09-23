#include "Process.h"

// Default constructor
Process::Process() : name("Unnamed Process"), currentLine(0), totalLines(0) {}

// Constructor
Process::Process(const string processName, int totalLines)
    : name(processName), currentLine(0), totalLines(totalLines) {}
