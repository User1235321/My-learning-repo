#ifndef BID_HPP
#define BID_HPP

enum class Priority {
    MASTER_SPORT = 1,
    CANDIDATE_MASTER = 2,
    REGULAR = 3
};

struct Bid {
    int id;
    int source_id;
    Priority priority;
    double arrival_time;
    double start_service_time;
    double end_service_time;
    
    Bid(int id, int source_id, Priority priority, double arrival_time);
    
    // Конструктор по умолчанию
    Bid() : id(0), source_id(0), priority(Priority::REGULAR), 
            arrival_time(0), start_service_time(0), end_service_time(0) {}
    
    // Метод для получения строкового представления приоритета
    const char* getPriorityString() const;
};

#endif
