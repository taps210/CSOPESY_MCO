#include "AConsole.h"
#include <string>

AConsole::AConsole(std::string name) : name(name)
{
}

std::string AConsole::getName()  {
    return this->name;
}