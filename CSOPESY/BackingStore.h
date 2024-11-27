#pragma once
#include "FlatMemoryAllocator.h"
#include <fstream>
#include <string>
#include <unordered_map>

class BackingStore : public FlatMemoryAllocator {
public:
    BackingStore(size_t maximumSize, const std::string& backingStorePath);

    void* allocate(size_t size, int processId) override;
    void swapOut(int processIdToEvict);
    void swapIn(int processId);

private:
    std::string backingStorePath;
    std::unordered_map<int, size_t> processLocations;

    void* swapAndAllocate(size_t size, int processId);
    int findProcessToEvict();
};