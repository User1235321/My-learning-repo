#ifndef SIMULATIONMODEL_HPP
#define SIMULATIONMODEL_HPP

#include "Source.hpp"
#include "Device.hpp"
#include "Buffer.hpp"
#include "EventCalendar.hpp"
#include <vector>
#include <map>
#include <atomic>
#include <mutex>

class SimulationModel {
private:
    std::vector<Source> sources;
    std::vector<Device> devices;
    Buffer buffer;
    EventCalendar calendar;
    
    std::atomic<double> current_time{0};
    std::atomic<bool> running{false};
    std::atomic<int> next_device_index{0};
    
    std::mutex console_mtx;
    std::mutex stats_mtx;
    
    struct Statistics {
        int total_generated = 0;
        int total_processed = 0;
        int total_rejected = 0;
        double total_service_time = 0;
        double total_waiting_time = 0;
        std::map<int, int> generated_per_source;
        std::map<int, int> rejected_per_source;
        std::map<int, double> service_time_per_source;
        std::map<int, double> waiting_time_per_source;
        std::map<int, double> device_busy_time;
    };
    
    Statistics stats;
    
    // Вспомогательные методы
    void printStepByStep(const std::string& message);
    void printCalendarState();
    void printBufferState();
    void printDeviceState();
    int getNextDevice();
    
    // Обработчики событий
    void handleArrival(const EventCalendar::Event& event);
    void handleServiceComplete(const EventCalendar::Event& event);
    void scheduleNextArrival(int source_id);
    void assignBidToDevice(const Bid& bid, int device_id);
    void printStatistics();
    
public:
    SimulationModel(int num_sources, int num_devices, size_t buffer_capacity,
                   double source_lambda, double device_min_time, double device_max_time);
    
    void runStepByStep();
    void runAutomatic(int max_events);
};

#endif
