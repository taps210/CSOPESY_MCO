#pragma once
#include <string>

class AConsole
{
public:
    AConsole(std::string name);
    ~AConsole() = default;

    std::string getName();
    virtual void onEnabled() = 0;
    virtual void display() = 0;
    virtual void process() = 0;

private:
    std::string name;
    friend class ConsoleManager;
};

