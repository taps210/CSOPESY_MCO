#pragma once
#include "ICommand.h"
#include <string>

class PrintCommand : public ICommand {
public:
	PrintCommand(int pid, std::string toPrint);
	// void execute(); 
	void execute(int cpuCoreId) override;
private:
	std::string toPrint;
};

