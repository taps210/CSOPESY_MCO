#include "BackingStore.h"

BackingStore::BackingStore(
    size_t maximumSize,
    const std::string& backingStorePath
) : FlatMemoryAllocator(maximumSize), backingStorePath(backingStorePath) {
    // Create or clear the backing store file
    std::ofstream backingStore(backingStorePath, std::ios::binary | std::ios::trunc);
    backingStore.close();
}

void* BackingStore::allocate(size_t size, int processId) {
    void* ptr = FlatMemoryAllocator::allocate(size, processId);

    if (ptr == nullptr) {
        // If allocation fails, attempt to swap out memory to backing store
        ptr = swapAndAllocate(size, processId);
    }

    return ptr;
}

void BackingStore::swapOut(int processIdToEvict) {
    std::ofstream backingStore(backingStorePath, std::ios::binary | std::ios::app);

    for (size_t i = 0; i < maximumSize; ++i) {
        if (processMap[i] == processIdToEvict) {
            // Write process memory to backing store
            backingStore.write(&memory[i], sizeof(char));

            // Mark memory as free
            allocationMap[i] = false;
            processMap[i] = -1;
        }
    }

    backingStore.close();
}

void BackingStore::swapIn(int processId) {
    std::ifstream backingStore(backingStorePath, std::ios::binary);
    std::vector<char> swappedMemory;

    // Read process memory from backing store
    char byte;
    while (backingStore.read(&byte, sizeof(char))) {
        swappedMemory.push_back(byte);
    }

    // Find space to allocate swapped memory
    void* newLocation = allocate(swappedMemory.size(), processId);
    if (newLocation) {
        std::copy(swappedMemory.begin(), swappedMemory.end(),
            static_cast<char*>(newLocation));
    }

    backingStore.close();
}

void* BackingStore::swapAndAllocate(size_t size, int processId) {
    // Find a process to swap out
    int processToEvict = findProcessToEvict();

    if (processToEvict != -1) {
        swapOut(processToEvict);
        return FlatMemoryAllocator::allocate(size, processId);
    }

    return nullptr;
}

int BackingStore::findProcessToEvict() {
    // Simple eviction strategy: evict the least recently used process
    for (const auto& entry : processMap) {
        if (entry.second != -1) {
            return entry.second;
        }
    }
    return -1;
}