#ifndef EVENTCALENDAR_HPP
#define EVENTCALENDAR_HPP

#include "Bid.hpp"
#include <queue>
#include <vector>
#include <string>
#include <mutex>
#include <functional>

class EventCalendar {
public:
    struct Event {
        double time;
        std::string type;
        int source_id;
        int device_id;
        Bid bid;
        
        bool operator>(const Event& other) const {
            return time > other.time;
        }
    };

private:
    std::priority_queue<Event, std::vector<Event>, std::greater<Event>> events;
    mutable std::mutex mtx;
    
public:
    void addEvent(double time, const std::string& type, int source_id, int device_id, const Bid& bid);
    Event getNextEvent();
    bool empty() const;
    size_t size() const;
};

#endif
