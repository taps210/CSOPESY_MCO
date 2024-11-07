#pragma once
#include "IMemoryAllocator.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <sstream>

class FlatMemoryAllocator : IMemoryAllocator {
public:
    FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0), memory(maximumSize, '.') {
        for (size_t i = 0; i < maximumSize; ++i) {
            allocationMap[i] = false;
        }
    }

    ~FlatMemoryAllocator() {
        memory.clear();
    }

    void* allocate(size_t size) override {
        // Find the first available block that can accommodate the process
        //cout << "\nSize to Allocate: " << size << endl;
        for (size_t i = 0; i < maximumSize - size + 1; ++i) {
            
            //cout << "memory size: " << memory.size() << endl;
            //cout << "i: " << i << endl;
            //cout << "AllocationMap: " << allocationMap[i] << endl;
            //cout << "Can Allocated?: " << canAllocateAt(i, size) << endl;

            if (!allocationMap[i] && canAllocateAt(i, size)) {
                //cout << "Allocating...\n" << endl;
                allocateAt(i, size);
                return &memory[i];
            }
        }

        // No available block found, return nullptr
        //cout << "Returning null\n\n" ;
        return nullptr;
    }

    void deallocate(void* ptr, size_t size) override {
        // Find the index of the memory block to deallocate
        size_t index = static_cast<char*>(ptr) - &memory[0];
        if (allocationMap[index]) {
            deallocateAt(index, size);
        }
    }

    std::string visualizeMemory() override {
        std::ostringstream oss;

        // Print upper boundary
        oss << "----end---- = " << maximumSize << "\n";

        // Iterate through memory and print process IDs or free space
        size_t currentAddress = maximumSize;
        for (size_t i = 0; i < memory.size(); ++i) {
            if (allocationMap[i]) {
                // Simulate process ID (e.g., P1, P2, etc.)
                oss << "P" << i + 1 << "\n";
                currentAddress -= 4096;  // Assuming each process takes up 4096 bytes (adjust as needed)
                oss << currentAddress << "\n";
            }
            else {
                // Free space
                oss << ".\n";
                currentAddress -= 16;  // Assuming each block of free space is 4096 bytes
                oss << currentAddress << "\n";
            }
        }

        // Print lower boundary
        oss << "----start---- = 0\n";

        return oss.str();
    }

    int getProcessCount() {
        return processCount;
    }

    size_t getExternalFragmentation() {
        return maximumSize - allocatedSize;
    }

private:
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory;
    std::unordered_map<size_t, bool> allocationMap;
    int processCount = 0;

    void initializeMemory() {
        std::fill(memory.begin(), memory.end(), '.'); // Reset memory to unallocated
        allocationMap.clear(); // Clear existing entries
        for (size_t i = 0; i < maximumSize; ++i) {
            allocationMap[i] = false;
        }
    }

    bool canAllocateAt(size_t index, size_t size) const {
        if (index + size > maximumSize) return false;
        for (size_t i = index; i < index + size; ++i) {
            if (allocationMap.find(i) != allocationMap.end() && allocationMap.at(i)) {
                return false;  // If any part of the block is already allocated, return false
            }
        }
        return true;
    }

    void allocateAt(size_t index, size_t size) {
        for (size_t i = index; i < index + size; ++i) {
            allocationMap[i] = true;
        }
        allocatedSize += size;
        processCount++;
    }

    void deallocateAt(size_t index, size_t size) {
        for (size_t i = index; i < index + size; ++i) {
            allocationMap[i] = false;
        }
        allocatedSize -= size;
        processCount--;
    }
};