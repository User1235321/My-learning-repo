#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "Bid.hpp"
#include <random>
#include <atomic>
#include <optional>

class Device {
private:
    int id;
    double min_service_time;
    double max_service_time;
    std::uniform_real_distribution<double> dist;
    std::mt19937 gen;
    std::atomic<bool> busy{false};
    std::optional<Bid> current_bid; // Используем optional вместо указателя
    
public:
    Device(int id, double min_time, double max_time);
    Device(const Device& other);
    
    double getServiceTime();
    bool isBusy() const;
    void setBusy(bool status);
    void setCurrentBid(const Bid& bid);
    std::optional<Bid> getCurrentBid() const;
    int getId() const;
};

#endif
