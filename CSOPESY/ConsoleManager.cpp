#include "ConsoleManager.h"
#include "MainConsole.h"

ConsoleManager::ConsoleManager() {
	this->running = true;
	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	this->consoleTable[MAIN_CONSOLE] = mainConsole;
}

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance() {
	return sharedInstance;
}

void ConsoleManager::initialize() {
	if (sharedInstance == nullptr) {
		sharedInstance = new ConsoleManager();
	}
	sharedInstance->switchConsole(MAIN_CONSOLE);
}

void ConsoleManager::destroy() {
	delete sharedInstance;
	sharedInstance = nullptr;
}

void ConsoleManager::drawConsole() {
	currentConsole->display();
}

void ConsoleManager::process() {
	currentConsole->process();
}

void ConsoleManager::switchConsole(const std::string consoleName) {
	if (!(this->consoleTable.contains(consoleName))) {
		std::cout << "Console name " << consoleName << " not found. \n";

		return;
	}
	system("cls");
	this->currentConsole = this->consoleTable[consoleName];
}

void ConsoleManager::registerScreen(std::shared_ptr<Screen> screenRef) {
    if (this->consoleTable.contains(screenRef->getName())) {
		throw std::runtime_error("Screen name '" + screenRef->getName() + "' already exists.\n");
		return;
    }
    consoleTable[screenRef->getName()] = screenRef;
}

void ConsoleManager::exitApplication() {
	this->running = false;
}

bool ConsoleManager::isRunning() {
	return running;
}