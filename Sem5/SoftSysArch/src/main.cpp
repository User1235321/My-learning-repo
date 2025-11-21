#include "SimulationModel.hpp"
#include <iostream>

int main() {
    std::cout << "Aikido Team Application Simulation System" << std::endl;
    std::cout << "Based on business domain by Pavel Rebdev" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    // Параметры системы согласно бизнес-домену
    int num_sources = 3;      // Количество потоков заявок (студентов)
    int num_devices = 2;      // Количество менеджеров
    size_t buffer_capacity = 5; // Вместимость буфера (серверов формы)
    double source_lambda = 0.5; // Интенсивность поступления заявок
    double device_min_time = 1.0; // Минимальное время обработки
    double device_max_time = 3.0; // Максимальное время обработки
    
    SimulationModel model(num_sources, num_devices, buffer_capacity, 
                         source_lambda, device_min_time, device_max_time);
    
    std::cout << "\nChoose mode:" << std::endl;
    std::cout << "1. Step-by-step mode" << std::endl;
    std::cout << "2. Automatic mode" << std::endl;
    std::cout << "Enter choice (1 or 2): ";
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) {
        std::cout << "\nStarting step-by-step simulation..." << std::endl;
        model.runStepByStep();
    } else if (choice == 2) {
        std::cout << "\nEnter maximum number of events: ";
        int max_events;
        std::cin >> max_events;
        std::cout << "Starting automatic simulation..." << std::endl;
        model.runAutomatic(max_events);
    } else {
        std::cout << "Invalid choice!" << std::endl;
    }
    
    return 0;
}
