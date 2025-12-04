#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include <map>

struct Good {
    int id;
    std::string name;
    int priority;
};

struct Sale {
    int id;
    int good_id;
    int good_count;
    std::string create_date;
    std::string good_name;
};

struct WarehouseStock {
    std::string good_name;
    int warehouse1_count;
    int warehouse2_count;
    int total_count;
};

class DatabaseManager {
public:
    static DatabaseManager& getInstance();
    
    bool connect(const std::string& host, const std::string& port, 
                 const std::string& dbname, const std::string& user, 
                 const std::string& password);
    bool isConnected() const;
    
    // Товары
    bool addGood(const std::string& name, int priority);
    bool updateGood(int id, const std::string& name, int priority);
    bool deleteGood(int id);
    std::vector<Good> getAllGoods();
    
    // Заявки
    bool addSale(int good_id, int count);
    bool updateSale(int id, int good_id, int count);
    bool deleteSale(int id);
    std::vector<Sale> getAllSales();
    
    // Склады
    bool updateWarehouse1(int good_id, int count);
    bool updateWarehouse2(int good_id, int count);
    std::vector<WarehouseStock> getWarehouseData();
    std::pair<int, int> getWarehouseStock(int good_id);
    
    // Отчеты
    std::vector<std::pair<std::string, int>> getTopGoods(const std::string& start_date, const std::string& end_date);
    std::vector<std::pair<std::string, int>> getDemandData(int good_id, const std::string& start_date, const std::string& end_date);

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    
    bool executeQuery(const std::string& query);
    
    bool connected_ = false;
    std::string connection_string_;
};

#endif
