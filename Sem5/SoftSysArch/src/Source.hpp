#ifndef SOURCE_HPP
#define SOURCE_HPP

#include "Bid.hpp"
#include <random>
#include <atomic>

class Source {
private:
    int id;
    double lambda;
    std::exponential_distribution<double> dist;
    std::mt19937 gen;
    std::atomic<int> bid_count{0};
    
public:
    Source(int id, double lambda);
    Source(const Source& other); // Конструктор копирования
    
    Bid generateBid(double current_time);
    double getNextArrivalTime();
    int getBidCount() const;
};

#endif
