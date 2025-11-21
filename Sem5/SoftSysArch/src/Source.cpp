#include "Source.hpp"
#include <random>

Source::Source(int id, double lambda) 
    : id(id), lambda(lambda), dist(lambda), gen(std::random_device{}()) {}

Source::Source(const Source& other)
    : id(other.id), lambda(other.lambda), dist(other.lambda), 
      gen(std::random_device{}()), bid_count(other.bid_count.load()) {}

Bid Source::generateBid(double current_time) {
    bid_count++;
    
    std::uniform_real_distribution<double> priority_dist(0.0, 1.0);
    double rand_val = priority_dist(gen);
    Priority priority;
    
    if (rand_val < 0.1) {
        priority = Priority::MASTER_SPORT;
    } else if (rand_val < 0.3) {
        priority = Priority::CANDIDATE_MASTER;
    } else {
        priority = Priority::REGULAR;
    }
    
    return Bid(bid_count, id, priority, current_time);
}

double Source::getNextArrivalTime() {
    return dist(gen);
}

int Source::getBidCount() const {
    return bid_count;
}
