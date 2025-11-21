#include "Buffer.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

Buffer::Buffer(size_t cap) : capacity(cap) {}

bool Buffer::addBid(const Bid& bid) {
    std::lock_guard<std::mutex> lock(mtx);
    if (buffer.size() >= capacity) {
        return false;
    }
    buffer.push_back(bid);
    return true;
}

Bid Buffer::removeOldest() {
    std::lock_guard<std::mutex> lock(mtx);
    if (buffer.empty()) {
        throw std::runtime_error("Buffer is empty - cannot remove oldest");
    }
    Bid oldest = buffer.front();
    buffer.erase(buffer.begin());
    return oldest;
}

Bid Buffer::getHighestPriority() {
    std::lock_guard<std::mutex> lock(mtx);
    if (buffer.empty()) {
        throw std::runtime_error("Buffer is empty - cannot get highest priority");
    }
    
    auto it = std::min_element(buffer.begin(), buffer.end(),
        [](const Bid& a, const Bid& b) {
            return static_cast<int>(a.priority) < static_cast<int>(b.priority);
        });
    
    Bid highest = *it;
    buffer.erase(it);
    return highest;
}

size_t Buffer::size() const {
    std::lock_guard<std::mutex> lock(mtx);
    return buffer.size();
}

size_t Buffer::getCapacity() const {
    return capacity;
}

std::vector<Bid> Buffer::getBufferState() const {
    std::lock_guard<std::mutex> lock(mtx);
    return buffer;
}
