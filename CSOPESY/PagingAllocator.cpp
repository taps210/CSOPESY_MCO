#include "PagingAllocator.h"
#include <iostream>
#include <algorithm>
#include <Process.h>

PagingAllocator::PagingAllocator(size_t maxMemorySize, size_t memPerFrame)
    : maxMemorySize(maxMemorySize), numFrames(maxMemorySize / memPerFrame) {
    for (size_t i = 0; i < numFrames; ++i) {
        freeFrameList.push_back(i);
    }
}

void* PagingAllocator::allocateBackingStore(std::shared_ptr<Process> process) {
    size_t processId = process->getPid();
    size_t numFramesNeeded = process->getPagesRequired();

    if (numFramesNeeded > freeFrameList.size()) {
        //std::cerr << "Memory allocation failed. Not enough free frames.\n";
        return nullptr;
    }

    // Attempt to allocate frames
    size_t frameIndex = allocateFrames(numFramesNeeded, processId);

    if (frameIndex == SIZE_MAX) {
        //std::cerr << "Memory allocation failed. Not enough free frames.\n";
        return nullptr; // No frames available
    }

    // If frames are not available in memory, handle the page fault
    // Here we assume a page fault occurs for each frame in the process
    for (size_t i = 0; i < numFramesNeeded; ++i) {
        size_t frame = frameIndex + i;
        if (frameMap.find(frame) == frameMap.end()) {
            handlePageFault(processId, frame); // Handle page fault
        }
    }

    return reinterpret_cast<void*>(frameIndex);
}

void* PagingAllocator::allocate(std::shared_ptr<Process> process) {
    size_t processId = process->getPid();
    size_t numFramesNeeded = process->getPagesRequired();

    if (numFramesNeeded > freeFrameList.size()) {
        //std::cerr << "Memory allocation failed. Not enough free frames.\n";
        return nullptr;
    }

    // Attempt to allocate frames
    size_t frameIndex = allocateFrames(numFramesNeeded, processId);

    if (frameIndex == SIZE_MAX) {
        //std::cerr << "Memory allocation failed. Not enough free frames.\n";
        return nullptr; // No frames available
    }

    // If frames are not available in memory, handle the page fault
    // Here we assume a page fault occurs for each frame in the process
    for (size_t i = 0; i < numFramesNeeded; ++i) {
        size_t frame = frameIndex + i;
        if (frameMap.find(frame) == frameMap.end()) {
            handlePageFault(processId, frame); // Handle page fault
        }
    }

    return reinterpret_cast<void*>(frameIndex);
}

void PagingAllocator::deallocate(std::shared_ptr<Process> process) {
    size_t processId = process->getPid();

    // Deallocate all frames allocated to this process
    for (auto it = frameMap.begin(); it != frameMap.end();) {
        if (it->second == processId) {
            size_t frameIndex = it->first;

            // Handle page fault (if necessary) before deallocation
            if (frameIndex == SIZE_MAX) {
                //std::cerr << "Page fault: Page is not in memory, needs to be loaded before deallocation.\n";
                handlePageFault(processId, frameIndex); // Handle page fault
            }

            it = frameMap.erase(it);  // Deallocate frame
            freeFrameList.push_back(frameIndex);  // Add to free list
        }
        else {
            ++it;
        }
    }
}

void PagingAllocator::handlePageFault(size_t processId, size_t frameId) {
    //std::cout << "Handling page fault for Process " << processId << ", Frame " << frameId << "\n";

    // Check if the page is in the backing store
    auto it = backingStore.find(processId);
    if (it != backingStore.end() && it->second.size() > frameId) {
        // Simulate loading the page from the backing store
        std::vector<char> page = it->second[frameId];

        // Allocate a new frame for the page fault
        size_t allocatedFrame = freeFrameList.back();
        freeFrameList.pop_back();
        frameMap[allocatedFrame] = processId;

        // Store the page in the allocated frame
        //std::cout << "Loaded page from backing store into Frame " << allocatedFrame << "\n";
    }
    else {
        //std::cerr << "Page not found in backing store.\n";
    }
}

std::string PagingAllocator::visualizeMemory() {
    std::string result = "Memory Visualization:\n";
    for (size_t frameIndex = 0; frameIndex < numFrames; ++frameIndex) {
        auto it = frameMap.find(frameIndex);
        if (it != frameMap.end()) {
            result += "Frame " + std::to_string(frameIndex) + " -> Process " +
                std::to_string(it->second) + "\n";
        }
        else {
            result += "Frame " + std::to_string(frameIndex) + " -> Free\n";
        }
    }
    result += "---------------------\n";
    return result;
}

size_t PagingAllocator::allocateFrames(size_t numFrames, size_t processId) {
    size_t startFrame = freeFrameList.back();
    for (size_t i = 0; i < numFrames; ++i) {
        size_t frameIndex = freeFrameList.back();
        freeFrameList.pop_back();
        frameMap[frameIndex] = processId;
    }
    return startFrame;
}

void PagingAllocator::deallocateFrames(size_t numFrames, size_t frameIndex) {
    for (size_t i = 0; i < numFrames; ++i) {
        frameMap.erase(frameIndex + i);
        freeFrameList.push_back(frameIndex + i);
    }
}
