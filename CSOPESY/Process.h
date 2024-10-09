#pragma once
#include <string>
#include <vector>

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
    //int getRemainingTime() const;
    int getCommandCounter() const;
    int getLinesOfCode() const;
    int getPid() const;
    int getCpuCoreId() const;
    ProcessState getState() const;
    string getName() const;

private:
    int pid;
    string name;
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter;
    int cpuCoreId = -1;
    ProcessState currentState;
};