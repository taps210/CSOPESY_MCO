#pragma once
#include "IMemoryAllocator.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <sstream>

class FlatMemoryAllocator : public IMemoryAllocator {
public:
    FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0), memory(maximumSize, '.') {
        for (size_t i = 0; i < maximumSize; ++i) {
            allocationMap[i] = false;
            processMap[i] = -1;
        }
    }

    ~FlatMemoryAllocator() {
        memory.clear();
    }

    void* allocate(shared_ptr<Process> process) override {
        // Find the first available block that can accommodate the process
        //cout << "\nSize to Allocate: " << size << endl;
        size_t size = process->getMemoryRequired();
        int processId = process->getPid();
        for (size_t i = 0; i < maximumSize - size + 1; ++i) {

            //cout << "memory size: " << memory.size() << endl;
            //cout << "i: " << i << endl;
            //cout << "AllocationMap: " << allocationMap[i] << endl;
            //cout << "Can Allocated?: " << canAllocateAt(i, size) << endl;

            if (!allocationMap[i] && canAllocateAt(i, size)) {
                allocateAt(i, size, processId);
                return &memory[i];
            }
        }
        return nullptr;
    }
    
    void deallocate(std::shared_ptr<Process> process) override {
        if (process->getMemoryPtr() == nullptr) {
            return; // No memory allocated for this process
        }

        size_t index = static_cast<char*>(process->getMemoryPtr()) - &memory[0]; // Correct index
        size_t size = process->getMemoryRequired();

        if (index + size <= maximumSize && allocationMap[index]) {
            deallocateAt(index, size);
            process->setMemoryPtr(nullptr); // Clear the pointer in the Process object
        }
    }


    std::string visualizeMemory() override {
        std::ostringstream oss;
        oss << "----end---- = " << maximumSize << "\n";

        size_t currentAddress = maximumSize;
        int i = maximumSize - 1;
        while (i > 0) {
            if (allocationMap[i]) {
                oss << i + 1 << "\n";
                oss << "P" << processMap[i] << "\n";
                i -= 4095;
                oss << i << "\n\n";
            }
            i--;
        }

        oss << "----start---- = 0\n";
        return oss.str();
    }

    int getProcessCount() {
        return processCount;
    }

    size_t getExternalFragmentation() {
        return maximumSize - allocatedSize;
    }

protected:
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory;
    std::unordered_map<size_t, int> allocationMap;
    std::unordered_map<size_t, int> processMap;
    int processCount = 0;

    void initializeMemory() {
        std::fill(memory.begin(), memory.end(), '.');
        allocationMap.clear();
        for (size_t i = 0; i < maximumSize; ++i) {
            allocationMap[i] = false;
            processMap[i] = -1;
        }
    }

    bool canAllocateAt(size_t index, size_t size) const {
        if (index + size > maximumSize) return false;
        for (size_t i = index; i < index + size; ++i) {
            if (allocationMap.find(i) != allocationMap.end() && allocationMap.at(i)) {
                return false;
            }
        }
        return true;
    }

    void allocateAt(size_t index, size_t size, int processId) {
        for (size_t i = index; i < index + size; ++i) {
            allocationMap[i] = true;
            processMap[i] = processId;
        }
        allocatedSize += size;
        processCount++;
    }

    void deallocateAt(size_t index, size_t size) {
        for (size_t i = index; i < index + size; ++i) {
            allocationMap[i] = false;
            processMap[i] = -1;
        }
        allocatedSize -= size;
        processCount--;
    }
};