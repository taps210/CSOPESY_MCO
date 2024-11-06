#pragma once
#include <unordered_map>

#include "CSOPESYThread.h"
#include "Process.h"
#include "AScheduler.h"
#include "FlatMemoryAllocator.h"


class GlobalScheduler : CSOPESYThread {
public:
	static GlobalScheduler* getInstance();
	static void initialize(int numCpu, std::string schedulerType, unsigned long int quantumCycles, unsigned long int batchProcessFreq, unsigned long int min, unsigned long int max, unsigned long int delaysPerExec);
	static void destroy();
	void run() override;

	std::shared_ptr<Process> createUniqueProcess(std::string processName);
	std::shared_ptr<Process> findProcess(std::string processName);

	// Week 6
	void tick();

	// Week 7
	std::string listProcesses() const;
	void createProcess();
	void logProcess() const;
	void setTester(bool test);
	bool getTester();

private:
	GlobalScheduler(int numCpu, std::string schedulerType, unsigned long int quantumCycles, unsigned long int batchProcessFreq, unsigned long int min, unsigned long int max, unsigned long int delaysPerExec);
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {}
	static GlobalScheduler* sharedInstance;
	std::shared_ptr<AScheduler> scheduler;
	std::unordered_map<string, std::shared_ptr<Process>> processes;
	int j = 0;
	int ticks;
	unsigned long int cpuCycles = 0;
	unsigned long int minCom = 0;
	unsigned long int maxCom = 0;
	bool tester = false;
	int workers;
	unsigned long int processFreq;
	unsigned long int execDelay;
};