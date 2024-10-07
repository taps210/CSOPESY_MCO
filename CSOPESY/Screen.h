#pragma once
#include <string>
#include <iostream>

#include "Process.h"
#include "AConsole.h"

class Screen : public AConsole
{
public:
    Screen(std::shared_ptr<Process> process);
    Screen();
    ~Screen() = default;
    void onEnabled() override;
    void process() override;
    void display() override;

private:
    std::shared_ptr<Process> attachedProcess;
    string timeCreated;
};
