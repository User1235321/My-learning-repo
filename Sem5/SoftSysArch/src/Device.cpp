#include "Device.hpp"
#include <random>

Device::Device(int id, double min_time, double max_time) 
    : id(id), min_service_time(min_time), max_service_time(max_time),
      dist(min_time, max_time), gen(std::random_device{}()) {}

Device::Device(const Device& other)
    : id(other.id), min_service_time(other.min_service_time), 
      max_service_time(other.max_service_time), dist(other.min_service_time, other.max_service_time),
      gen(std::random_device{}()), busy(other.busy.load()), current_bid(other.current_bid) {}

double Device::getServiceTime() {
    return dist(gen);
}

bool Device::isBusy() const {
    return busy;
}

void Device::setBusy(bool status) {
    busy = status;
    if (!status) {
        current_bid.reset(); // Сбрасываем заявку при освобождении устройства
    }
}

void Device::setCurrentBid(const Bid& bid) {
    current_bid = bid;
}

std::optional<Bid> Device::getCurrentBid() const {
    return current_bid;
}

int Device::getId() const {
    return id;
}
