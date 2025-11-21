#include "Bid.hpp"
#include <stdexcept>

Bid::Bid(int id, int source_id, Priority priority, double arrival_time)
    : id(id), source_id(source_id), priority(priority), arrival_time(arrival_time),
      start_service_time(0), end_service_time(0) {}

const char* Bid::getPriorityString() const {
    switch (priority) {
        case Priority::MASTER_SPORT: return "MASTER_SPORT";
        case Priority::CANDIDATE_MASTER: return "CANDIDATE_MASTER";
        case Priority::REGULAR: return "REGULAR";
        default: throw std::runtime_error("Unknown priority");
    }
}
