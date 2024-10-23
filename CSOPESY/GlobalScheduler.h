#pragma once
#include <unordered_map>

#include "CSOPESYThread.h"
#include "Process.h"
#include "AScheduler.h"


class GlobalScheduler : CSOPESYThread {
public:
	static GlobalScheduler* getInstance();
	static void initialize();
	static void destroy();
	void run() override;

	std::shared_ptr<Process> createUniqueProcess(std::string processName);
	std::shared_ptr<Process> findProcess(std::string processName);

	// Week 6
	void create10Processes();
	void listProcesses() const;
	void tick() const;
	std::string listProcesses() const;
	void logProcess() const;

	// Week 7
	void setNumCpus(int numCpu);
	void createProcess();

private:
	GlobalScheduler();
	~GlobalScheduler() = default;
	GlobalScheduler(GlobalScheduler const&) {};
	GlobalScheduler& operator=(GlobalScheduler const&) {}
	static GlobalScheduler* sharedInstance;
	std::shared_ptr<AScheduler> scheduler;
	std::unordered_map<string, std::shared_ptr<Process>> processes;
	int j = 0;
};