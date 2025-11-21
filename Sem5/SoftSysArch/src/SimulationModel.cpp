#include "SimulationModel.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <algorithm>

SimulationModel::SimulationModel(int num_sources, int num_devices, size_t buffer_capacity,
                   double source_lambda, double device_min_time, double device_max_time)
    : buffer(buffer_capacity) {
    
    // Используем push_back вместо emplace_back
    for (int i = 0; i < num_sources; ++i) {
        sources.push_back(Source(i + 1, source_lambda));
    }
    
    for (int i = 0; i < num_devices; ++i) {
        devices.push_back(Device(i + 1, device_min_time, device_max_time));
    }
}

void SimulationModel::runStepByStep() {
    running = true;
    current_time = 0;
    
    // Инициализация первых событий прибытия
    for (auto& source : sources) {
        double arrival_time = source.getNextArrivalTime();
        Bid bid = source.generateBid(arrival_time);
        calendar.addEvent(arrival_time, "ARRIVAL", bid.source_id, -1, bid);
    }
    
    printStepByStep("Simulation started in step-by-step mode");
    
    while (!calendar.empty() && running) {
        auto event = calendar.getNextEvent();
        current_time = event.time;
        
        if (event.type == "ARRIVAL") {
            handleArrival(event);
        } else if (event.type == "SERVICE_COMPLETE") {
            handleServiceComplete(event);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    printStepByStep("Simulation completed");
    printStatistics();
}

void SimulationModel::runAutomatic(int max_events) {
    running = true;
    current_time = 0;
    int event_count = 0;
    
    // Инициализация первых событий прибытия
    for (auto& source : sources) {
        double arrival_time = source.getNextArrivalTime();
        Bid bid = source.generateBid(arrival_time);
        calendar.addEvent(arrival_time, "ARRIVAL", bid.source_id, -1, bid);
    }
    
    std::cout << "Simulation started in automatic mode" << std::endl;
    
    while (!calendar.empty() && running && event_count < max_events) {
        auto event = calendar.getNextEvent();
        current_time = event.time;
        event_count++;
        
        if (event.type == "ARRIVAL") {
            handleArrival(event);
        } else if (event.type == "SERVICE_COMPLETE") {
            handleServiceComplete(event);
        }
    }
    
    std::cout << "Simulation completed after " << event_count << " events" << std::endl;
    printStatistics();
}

void SimulationModel::printStepByStep(const std::string& message) {
    std::lock_guard<std::mutex> lock(console_mtx);
    std::cout << "Time: " << std::fixed << std::setprecision(2) << current_time 
              << " | " << message << std::endl;
}

void SimulationModel::printCalendarState() {
    std::lock_guard<std::mutex> lock(console_mtx);
    std::cout << "=== Calendar State ===" << std::endl;
    std::cout << "Events in calendar: " << calendar.size() << std::endl;
}

void SimulationModel::printBufferState() {
    auto buffer_state = buffer.getBufferState();
    std::lock_guard<std::mutex> lock(console_mtx);
    std::cout << "=== Buffer State ===" << std::endl;
    std::cout << "Buffer usage: " << buffer_state.size() << "/" << buffer.getCapacity() << std::endl;
    for (const auto& bid : buffer_state) {
        std::cout << "Bid " << bid.id << " from source " << bid.source_id 
                  << " (Priority: " << bid.getPriorityString() << ")" << std::endl;
    }
}

void SimulationModel::printDeviceState() {
    std::lock_guard<std::mutex> lock(console_mtx);
    std::cout << "=== Device State ===" << std::endl;
    for (const auto& device : devices) {
        std::cout << "Device " << device.getId() << ": " 
                  << (device.isBusy() ? "BUSY" : "FREE");
        auto current_bid = device.getCurrentBid();
        if (device.isBusy() && current_bid.has_value()) {
            std::cout << " processing Bid " << current_bid.value().id;
        }
        std::cout << std::endl;
    }
}

int SimulationModel::getNextDevice() {
    int device_id = next_device_index;
    next_device_index = (next_device_index + 1) % devices.size();
    return device_id;
}

void SimulationModel::handleArrival(const EventCalendar::Event& event) {
    auto bid = event.bid;
    
    {
        std::lock_guard<std::mutex> lock(stats_mtx);
        stats.total_generated++;
        stats.generated_per_source[bid.source_id]++;
    }
    
    printStepByStep("Bid " + std::to_string(bid.id) + " arrived from source " + 
                   std::to_string(bid.source_id) + " (Priority: " + bid.getPriorityString() + ")");
    
    scheduleNextArrival(bid.source_id);
    
    bool assigned = false;
    for (auto& device : devices) {
        if (!device.isBusy()) {
            assignBidToDevice(bid, device.getId());
            assigned = true;
            break;
        }
    }
    
    if (!assigned) {
        if (buffer.addBid(bid)) {
            printStepByStep("Bid " + std::to_string(bid.id) + " placed in buffer");
        } else {
            try {
                auto oldest_bid = buffer.removeOldest();
                buffer.addBid(bid);
                
                {
                    std::lock_guard<std::mutex> lock(stats_mtx);
                    stats.total_rejected++;
                    stats.rejected_per_source[oldest_bid.source_id]++;
                }
                
                printStepByStep("Buffer full! Oldest bid " + std::to_string(oldest_bid.id) + 
                               " rejected, new bid " + std::to_string(bid.id) + " added");
            } catch (const std::exception& e) {
                printStepByStep("Error removing oldest bid: " + std::string(e.what()));
            }
        }
    }
    
    printCalendarState();
    printBufferState();
    printDeviceState();
    std::cout << std::endl;
}

void SimulationModel::handleServiceComplete(const EventCalendar::Event& event) {
    auto device_id = event.device_id;
    auto bid = event.bid;
    
    {
        std::lock_guard<std::mutex> lock(stats_mtx);
        stats.total_processed++;
        double service_time = bid.end_service_time - bid.start_service_time;
        double waiting_time = bid.start_service_time - bid.arrival_time;
        stats.total_service_time += service_time;
        stats.total_waiting_time += waiting_time;
        stats.service_time_per_source[bid.source_id] += service_time;
        stats.waiting_time_per_source[bid.source_id] += waiting_time;
        stats.device_busy_time[device_id] += service_time;
    }
    
    printStepByStep("Device " + std::to_string(device_id) + 
                   " completed processing bid " + std::to_string(bid.id));
    
    for (auto& device : devices) {
        if (device.getId() == device_id) {
            device.setBusy(false);
            break;
        }
    }
    
    if (buffer.size() > 0) {
        try {
            auto next_bid = buffer.getHighestPriority();
            assignBidToDevice(next_bid, device_id);
        } catch (const std::exception& e) {
            // Игнорируем исключение если буфер пуст
        }
    }
    
    printCalendarState();
    printBufferState();
    printDeviceState();
    std::cout << std::endl;
}

void SimulationModel::scheduleNextArrival(int source_id) {
    // Исправляем предупреждение о сравнении знаковых и беззнаковых типов
    if (source_id >= 1 && static_cast<size_t>(source_id - 1) < sources.size()) {
        double next_arrival = current_time + sources[source_id - 1].getNextArrivalTime();
        Bid next_bid = sources[source_id - 1].generateBid(next_arrival);
        calendar.addEvent(next_arrival, "ARRIVAL", source_id, -1, next_bid);
    }
}

void SimulationModel::assignBidToDevice(const Bid& bid, int device_id) {
    for (auto& device : devices) {
        if (device.getId() == device_id && !device.isBusy()) {
            double service_time = device.getServiceTime();
            // Исправляем ошибку с std::max - используем load() для atomic
            double current_time_value = current_time.load();
            double start_time = std::max(current_time_value, bid.arrival_time);
            double end_time = start_time + service_time;
            
            // Создаем копию заявки для обработки
            Bid processing_bid = bid;
            processing_bid.start_service_time = start_time;
            processing_bid.end_service_time = end_time;
            
            device.setBusy(true);
            device.setCurrentBid(processing_bid);
            
            calendar.addEvent(end_time, "SERVICE_COMPLETE", bid.source_id, device_id, processing_bid);
            
            printStepByStep("Bid " + std::to_string(bid.id) + 
                           " assigned to device " + std::to_string(device_id) +
                           ", service time: " + std::to_string(service_time));
            break;
        }
    }
}

void SimulationModel::printStatistics() {
    std::lock_guard<std::mutex> lock(console_mtx);
    std::cout << "\n=== FINAL STATISTICS ===" << std::endl;
    std::cout << "Total bids generated: " << stats.total_generated << std::endl;
    std::cout << "Total bids processed: " << stats.total_processed << std::endl;
    std::cout << "Total bids rejected: " << stats.total_rejected << std::endl;
    
    if (stats.total_processed > 0) {
        std::cout << "Average service time: " << stats.total_service_time / stats.total_processed << std::endl;
        std::cout << "Average waiting time: " << stats.total_waiting_time / stats.total_processed << std::endl;
    }
    
    std::cout << "\nPer-source statistics:" << std::endl;
    for (const auto& [source_id, generated] : stats.generated_per_source) {
        int rejected = stats.rejected_per_source[source_id];
        double rejection_rate = (generated > 0) ? (static_cast<double>(rejected) / generated) * 100 : 0;
        
        std::cout << "Source " << source_id << ": " << generated << " generated, "
                  << rejected << " rejected (" << std::fixed << std::setprecision(2) 
                  << rejection_rate << "%)" << std::endl;
    }
    
    std::cout << "\nDevice utilization:" << std::endl;
    for (const auto& [device_id, busy_time] : stats.device_busy_time) {
        double current_time_value = current_time.load();
        double utilization = (current_time_value > 0) ? (busy_time / current_time_value) * 100 : 0;
        std::cout << "Device " << device_id << ": " << std::fixed << std::setprecision(2) 
                  << utilization << "%" << std::endl;
    }
}
