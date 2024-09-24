#pragma once

#include "ScreenHandler.h"
#include "ProcessHandler.h"
class ConsoleManager
{

private: 
	string command;
	string mode = "menu";

public:
	ConsoleManager();

	void runCommand();

};

