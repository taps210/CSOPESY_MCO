#pragma once
#include "AConsole.h"
#include <string>

class MainConsole : public AConsole {
public:
	MainConsole(std::string consoleName) : AConsole(consoleName) {}
	MainConsole() : AConsole("") {}
	~MainConsole() = default;
	void onEnabled() override;
	void process() override;
	void display() override;
	void initializeSystem();
	void configureScheduler(int numCpu, const std::string& schedulerType, int quantumCycles, int batchProcessFreq, int minIns, int maxIns, int delaysPerExec);

private:
	bool initialized = false;
	bool tester = false;
};