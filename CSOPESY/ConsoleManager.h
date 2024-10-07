#pragma once
#include <unordered_map>
#include "Screen.h"
#include "AConsole.h"

const std::string MAIN_CONSOLE = "MAIN_CONSOLE";

class ConsoleManager
{
public:
	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	void drawConsole();
	void process();
	void switchConsole(std::string consoleName);

	void registerScreen(std::shared_ptr<Screen> screenRef);

	void exitApplication();
	bool isRunning();

private:
	ConsoleManager();
	~ConsoleManager() = default;
	ConsoleManager(ConsoleManager const&) {}
	ConsoleManager& operator=(ConsoleManager const&) {}
	static ConsoleManager* sharedInstance;
	std::unordered_map<std::string, std::shared_ptr<AConsole>> consoleTable;

	std::shared_ptr<AConsole> currentConsole;
	bool running = true;
};