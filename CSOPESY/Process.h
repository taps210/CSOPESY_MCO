#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Process {
public:

    enum ProcessState {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    Process(int pid, string name);

    void setCommands(unsigned long int numOfCommands);
    void executeCurrentCommand() const;
    void moveToNextLine();

    bool isFinished() const;
    int getRemainingTime();
    unsigned long int getCommandCounter() const;
    unsigned long int getLinesOfCode() const;
    int getPid() const;
    int getCpuCoreId() const;
    void setRemainingTime(int _remainingTime);
    void decrementRemainingTime();
    ProcessState getState() const;
    string getName() const;
    string getTimeCreated() const;

    void setState(Process::ProcessState processState);
    void setCpuCoreId(int _cpuCoreId);

    size_t getMemoryRequired();
    size_t getNumPages();
    void* getMemoryPtr();
    void setMemoryRequired(size_t size);
    void setMemoryPtr(void* ptr);

private:
    int pid;
    string name;
    unsigned long int commands = 0;

    unsigned long int commandCounter;
    int cpuCoreId = -1;
    int remainingTime = 5;
    ProcessState currentState;
    string timeCreated;

    size_t memoryRequired = 4096;
    size_t numPages = 2;
    void* memoryPtr = nullptr;
};