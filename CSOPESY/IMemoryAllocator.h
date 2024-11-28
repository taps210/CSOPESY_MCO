#pragma once
#include <string>
#include "Process.h"
class IMemoryAllocator {
public:
	virtual void* allocateBackingStore(shared_ptr<Process> p) = 0;
	virtual void* allocate(shared_ptr<Process> p) = 0;
	virtual void deallocate(shared_ptr<Process> p) = 0;
	virtual std::string visualizeMemory() = 0;
};

