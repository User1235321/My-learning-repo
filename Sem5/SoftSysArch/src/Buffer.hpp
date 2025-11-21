#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Bid.hpp"
#include <vector>
#include <mutex>

class Buffer {
private:
    std::vector<Bid> buffer;
    size_t capacity;
    mutable std::mutex mtx;
    
public:
    Buffer(size_t cap);
    
    // Основные операции с буфером
    bool addBid(const Bid& bid);
    Bid removeOldest();
    Bid getHighestPriority();
    
    // Методы для получения состояния
    size_t size() const;
    size_t getCapacity() const;
    std::vector<Bid> getBufferState() const;
};

#endif
