#include "DatabaseManager.h"
#include <iostream>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connect(const std::string& host, const std::string& port, 
                            const std::string& dbname, const std::string& user, 
                            const std::string& password) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(QString::fromStdString(host));
    db.setPort(std::stoi(port));
    db.setDatabaseName(QString::fromStdString(dbname));
    db.setUserName(QString::fromStdString(user));
    db.setPassword(QString::fromStdString(password));
    
    connected_ = db.open();
    if (connected_) {
        std::cout << "Connected to database successfully" << std::endl;
    } else {
        QMessageBox::critical(nullptr, "Ошибка", 
                            "Ошибка подключения к БД: " + db.lastError().text());
    }
    return connected_;
}

bool DatabaseManager::isConnected() const {
    return connected_;
}

bool DatabaseManager::executeQuery(const std::string& query) {
    if (!connected_) return false;
    QSqlQuery q;
    return q.exec(QString::fromStdString(query));
}

bool DatabaseManager::goodExists(const std::string& name) {
    if (!connected_) return false;
    
    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM goods WHERE LOWER(name) = LOWER(?)");
    q.addBindValue(QString::fromStdString(name));
    
    if (q.exec() && q.next()) {
        return q.value(0).toInt() > 0;
    }
    return false;
}

bool DatabaseManager::goodExistsWithDifferentId(const std::string& name, int excludeId) {
    if (!connected_) return false;
    
    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM goods WHERE LOWER(name) = LOWER(?) AND id != ?");
    q.addBindValue(QString::fromStdString(name));
    q.addBindValue(excludeId);
    
    if (q.exec() && q.next()) {
        return q.value(0).toInt() > 0;
    }
    return false;
}

bool DatabaseManager::addGood(const std::string& name, int priority) {
    if (goodExists(name)) {
        std::cerr << "Товар с именем '" << name << "' уже существует!" << std::endl;
        return false;
    }
    
    QSqlQuery q;
    q.prepare("INSERT INTO goods (name, priority) VALUES (?, ?)");
    q.addBindValue(QString::fromStdString(name));
    q.addBindValue(priority);
    return q.exec();
}

bool DatabaseManager::updateGood(int id, const std::string& name, int priority) {
    if (goodExistsWithDifferentId(name, id)) {
        std::cerr << "Товар с именем '" << name << "' уже существует!" << std::endl;
        return false;
    }
    
    QSqlQuery q;
    q.prepare("UPDATE goods SET name = ?, priority = ? WHERE id = ?");
    q.addBindValue(QString::fromStdString(name));
    q.addBindValue(priority);
    q.addBindValue(id);
    return q.exec();
}

bool DatabaseManager::deleteGood(int id) {
    QSqlQuery q;
    q.prepare("DELETE FROM goods WHERE id = ?");
    q.addBindValue(id);
    return q.exec();
}

std::vector<Good> DatabaseManager::getAllGoods() {
    std::vector<Good> goods;
    if (!connected_) return goods;
    
    QSqlQuery q("SELECT id, name, priority FROM goods ORDER BY name");
    while (q.next()) {
        Good good;
        good.id = q.value(0).toInt();
        good.name = q.value(1).toString().toStdString();
        good.priority = q.value(2).toInt();
        goods.push_back(good);
    }
    return goods;
}

bool DatabaseManager::addSale(int good_id, int count) {
    // Получаем текущее количество товара на складах
    int availableStock = getTotalStockForGood(good_id);
    
    if (count > availableStock) {
        std::cerr << "Недостаточно товара на складах. Запрошено: " << count 
                  << ", доступно: " << availableStock << std::endl;
        return false;
    }
    
    QSqlQuery q;
    q.prepare("INSERT INTO sales (good_id, good_count, create_date) VALUES (?, ?, CURRENT_DATE)");
    q.addBindValue(good_id);
    q.addBindValue(count);
    return q.exec();
}

bool DatabaseManager::updateSale(int id, int good_id, int count) {
    // Для обновления тоже нужно проверить доступность товара
    int availableStock = getTotalStockForGood(good_id);
    
    // Получаем текущее количество в этой заявке
    QSqlQuery getCurrentSale;
    getCurrentSale.prepare("SELECT good_count FROM sales WHERE id = ?");
    getCurrentSale.addBindValue(id);
    
    int currentCount = 0;
    if (getCurrentSale.exec() && getCurrentSale.next()) {
        currentCount = getCurrentSale.value(0).toInt();
    }
    
    // Вычисляем изменение количества
    int difference = count - currentCount;
    
    if (difference > availableStock) {
        std::cerr << "Недостаточно товара на складах для увеличения заявки." << std::endl;
        return false;
    }
    
    QSqlQuery q;
    q.prepare("UPDATE sales SET good_id = ?, good_count = ? WHERE id = ?");
    q.addBindValue(good_id);
    q.addBindValue(count);
    q.addBindValue(id);
    return q.exec();
}

bool DatabaseManager::deleteSale(int id) {
    QSqlQuery q;
    q.prepare("DELETE FROM sales WHERE id = ?");
    q.addBindValue(id);
    return q.exec();
}

std::vector<Sale> DatabaseManager::getAllSales() {
    std::vector<Sale> sales;
    if (!connected_) return sales;
    
    QSqlQuery q("SELECT s.id, g.name, s.good_id, s.good_count, s.create_date "
                "FROM sales s JOIN goods g ON s.good_id = g.id "
                "ORDER BY s.create_date DESC");
    while (q.next()) {
        Sale sale;
        sale.id = q.value(0).toInt();
        sale.good_name = q.value(1).toString().toStdString();
        sale.good_id = q.value(2).toInt();
        sale.good_count = q.value(3).toInt();
        sale.create_date = q.value(4).toString().toStdString();
        sales.push_back(sale);
    }
    return sales;
}

bool DatabaseManager::updateWarehouse1(int good_id, int count) {
    QSqlQuery q;
    q.prepare("INSERT INTO warehouse1 (good_id, good_count) VALUES (?, ?) "
              "ON CONFLICT (good_id) DO UPDATE SET good_count = ?");
    q.addBindValue(good_id);
    q.addBindValue(count);
    q.addBindValue(count);
    return q.exec();
}

bool DatabaseManager::updateWarehouse2(int good_id, int count) {
    QSqlQuery q;
    q.prepare("INSERT INTO warehouse2 (good_id, good_count) VALUES (?, ?) "
              "ON CONFLICT (good_id) DO UPDATE SET good_count = ?");
    q.addBindValue(good_id);
    q.addBindValue(count);
    q.addBindValue(count);
    return q.exec();
}

std::vector<WarehouseStock> DatabaseManager::getWarehouseData() {
    std::vector<WarehouseStock> stocks;
    if (!connected_) return stocks;
    
    QSqlQuery q("SELECT g.name, "
                "COALESCE(w1.good_count, 0) as warehouse1_count, "
                "COALESCE(w2.good_count, 0) as warehouse2_count, "
                "COALESCE(w1.good_count, 0) + COALESCE(w2.good_count, 0) as total_count "
                "FROM goods g "
                "LEFT JOIN warehouse1 w1 ON g.id = w1.good_id "
                "LEFT JOIN warehouse2 w2 ON g.id = w2.good_id");
    
    while (q.next()) {
        WarehouseStock stock;
        stock.good_name = q.value(0).toString().toStdString();
        stock.warehouse1_count = q.value(1).toInt();
        stock.warehouse2_count = q.value(2).toInt();
        stock.total_count = q.value(3).toInt();
        stocks.push_back(stock);
    }
    return stocks;
}

std::pair<int, int> DatabaseManager::getWarehouseStock(int good_id) {
    if (!connected_) return {0, 0};
    
    QSqlQuery q1;
    q1.prepare("SELECT good_count FROM warehouse1 WHERE good_id = ?");
    q1.addBindValue(good_id);
    
    int count1 = 0, count2 = 0;
    if (q1.exec() && q1.next()) {
        count1 = q1.value(0).toInt();
    }
    
    QSqlQuery q2;
    q2.prepare("SELECT good_count FROM warehouse2 WHERE good_id = ?");
    q2.addBindValue(good_id);
    
    if (q2.exec() && q2.next()) {
        count2 = q2.value(0).toInt();
    }
    
    return {count1, count2};
}

int DatabaseManager::getTotalStockForGood(int good_id) {
    if (!connected_) return 0;
    
    QSqlQuery q;
    q.prepare("SELECT "
              "COALESCE(w1.good_count, 0) + COALESCE(w2.good_count, 0) as total_count "
              "FROM goods g "
              "LEFT JOIN warehouse1 w1 ON g.id = w1.good_id "
              "LEFT JOIN warehouse2 w2 ON g.id = w2.good_id "
              "WHERE g.id = ?");
    q.addBindValue(good_id);
    
    if (q.exec() && q.next()) {
        return q.value(0).toInt();
    }
    return 0;
}

std::vector<std::pair<std::string, int>> DatabaseManager::getTopGoods(const std::string& start_date, const std::string& end_date) {
    std::vector<std::pair<std::string, int>> result;
    if (!connected_) return result;
    
    QSqlQuery q;
    q.prepare("SELECT g.name, SUM(s.good_count) as total_sold "
              "FROM sales s "
              "JOIN goods g ON s.good_id = g.id "
              "WHERE s.create_date BETWEEN ? AND ? "
              "GROUP BY g.name "
              "ORDER BY total_sold DESC "
              "LIMIT 5");
    q.addBindValue(QString::fromStdString(start_date));
    q.addBindValue(QString::fromStdString(end_date));
    
    if (q.exec()) {
        while (q.next()) {
            result.emplace_back(
                q.value(0).toString().toStdString(),
                q.value(1).toInt()
            );
        }
    }
    return result;
}

std::vector<std::pair<std::string, int>> DatabaseManager::getDemandData(int good_id, const std::string& start_date, const std::string& end_date) {
    std::vector<std::pair<std::string, int>> result;
    if (!connected_) return result;
    
    QSqlQuery q;
    q.prepare("SELECT create_date, SUM(good_count) as daily_sold "
              "FROM sales "
              "WHERE good_id = ? AND create_date BETWEEN ? AND ? "
              "GROUP BY create_date "
              "ORDER BY create_date");
    q.addBindValue(good_id);
    q.addBindValue(QString::fromStdString(start_date));
    q.addBindValue(QString::fromStdString(end_date));
    
    if (q.exec()) {
        while (q.next()) {
            result.emplace_back(
                q.value(0).toString().toStdString(),
                q.value(1).toInt()
            );
        }
    }
    return result;
}
