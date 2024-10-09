#include <thread>

#include "CSOPESYThread.h"


void CSOPESYThread::start() {
	std::thread(&CSOPESYThread::run, this).detach();
}

void CSOPESYThread::sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}