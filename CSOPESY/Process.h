#pragma once
#include <string>
#include <vector>
#include <memory>

#include "ICommand.h"
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

    void addCommand(ICommand::CommandType commandType);
    void executeCurrentCommand() const;
    void moveToNextLine();

    bool isFinished() const;
    int getRemainingTime();
    int getCommandCounter() const;
    int getLinesOfCode() const;
    int getPid() const;
    int getCpuCoreId() const;
    void setRemainingTime(int _remainingTime);
    void decrementRemainingTime();
    ProcessState getState() const;
    string getName() const;
    string getTimeCreated() const;

    void setState(Process::ProcessState processState);
    void setCpuCoreId(int _cpuCoreId);

private:
    int pid;
    string name;
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter;
    int cpuCoreId = -1;
    int remainingTime = 0;
    ProcessState currentState;
    string timeCreated;
};