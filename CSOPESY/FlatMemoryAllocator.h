#include "IMemoryAllocator.h"
#include <vector>
#include <unordered_map>

class FlatMemoryAllocator : public IMemoryAllocator {
public:
    FlatMemoryAllocator(size_t maximumSize) : maximumSize(maximumSize), allocatedSize(0) {
        memory.reserve(maximumSize);
        initializeMemory();
    }

    ~FlatMemoryAllocator() {
        memory.clear();
    }

    void* allocate(size_t size) override {
        // Find the first available block that can accommodate the process
        for (size_t i = 0; i < maximumSize - size + 1; ++i) {
            if (allocationMap[i] && canAllocateAt(i, size)) {
                allocateAt(i, size);
                return &memory[i];
            }
        }

        // No available block found, return nullptr
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
        return std::string(memory.begin(), memory.end());
    }

private:
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory;
    std::unordered_map<size_t, bool> allocationMap;

    void initializeMemory() {
        std::fill(memory.begin(), memory.end(), '.'); // '.' represents unallocated memory
        std::fill(allocationMap.begin(), allocationMap.end(), false);
    }

    bool canAllocateAt(size_t index, size_t size) const {
        // Check if the memory block is large enough
        return (index + size <= maximumSize);
    }

    void allocateAt(size_t index, size_t size) {
        // Mark the memory block as allocated
        std::fill(allocationMap.begin()->first + index, allocationMap.begin()->first + index + size, true);
        allocatedSize += size;
    }

    void deallocateAt(size_t index) {
        // Mark the memory block as deallocated
        allocationMap[index] = false;
    }
};