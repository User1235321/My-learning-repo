#include "EventCalendar.hpp"
#include <stdexcept>

void EventCalendar::addEvent(double time, const std::string& type, int source_id, int device_id, const Bid& bid) {
    std::lock_guard<std::mutex> lock(mtx);
    events.push({time, type, source_id, device_id, bid});
}

EventCalendar::Event EventCalendar::getNextEvent() {
    std::lock_guard<std::mutex> lock(mtx);
    if (events.empty()) {
        throw std::runtime_error("No events in calendar");
    }
    Event next = events.top();
    events.pop();
    return next;
}

bool EventCalendar::empty() const {
    std::lock_guard<std::mutex> lock(mtx);
    return events.empty();
}

size_t EventCalendar::size() const {
    std::lock_guard<std::mutex> lock(mtx);
    return events.size();
}
