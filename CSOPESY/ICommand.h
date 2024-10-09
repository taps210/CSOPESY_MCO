#pragma once
class ICommand
{
public:
	enum CommandType
	{
		IO,
		PRINT
	};

	ICommand(int pid, CommandType commandType);
	CommandType getCommandType();
	virtual void execute() = 0;
protected:
	int pid;
	CommandType commandType;
};

