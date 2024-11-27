#pragma once
#include "FlatMemoryAllocator.h"
#include <fstream>
#include <string>
#include <unordered_map>

class BackingStore : public FlatMemoryAllocator {
public:
    BackingStore(size_t maximumSize, const std::string& backingStorePath);

    void* allocate(shared_ptr<Process> p) override;
    void swapOut(int processIdToEvict);
    void swapIn(shared_ptr<Process> p);

private:
    std::string backingStorePath;
    std::unordered_map<int, size_t> processLocations;

    void* swapAndAllocate(shared_ptr<Process> p);
    int findProcessToEvict();
};