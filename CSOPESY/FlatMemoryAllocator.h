#pragma once
#include "IMemoryAllocator.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

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

    void deallocate(void* ptr) override {
        // Find the index of the memory block to deallocate
        size_t index = static_cast<char*>(ptr) - &memory[0];
        if (allocationMap[index]) {
            deallocateAt(index);
        }
    }

    std::string visualizeMemory() override {
        return std::to_string(allocatedSize);
    }

private:
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory;
    std::unordered_map<size_t, bool> allocationMap;

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
    }

    void deallocateAt(size_t index) {
        allocationMap[index] = false;
    }
};