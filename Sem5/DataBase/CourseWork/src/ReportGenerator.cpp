#include "ReportGenerator.h"
#include "DatabaseManager.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>

void ReportGenerator::generateTopGoodsReport() {
    try {
        // Получаем текущую дату для отчета
        time_t now = time(0);
        tm* localtm = localtime(&now);
        char end_date[11];
        strftime(end_date, sizeof(end_date), "%Y-%m-%d", localtm);
        
        // Берем данные за последние 30 дней
        tm start_tm = *localtm;
        start_tm.tm_mday -= 30;
        mktime(&start_tm);
        char start_date[11];
        strftime(start_date, sizeof(start_date), "%Y-%m-%d", &start_tm);
        
        auto result = DatabaseManager::getInstance().getTopGoods(start_date, end_date);
        
        std::ofstream file("top_goods_report.txt");
        if (!file.is_open()) {
            std::cerr << "Cannot open file for writing" << std::endl;
            return;
        }
        
        file << "========================================\n";
        file << "     ОТЧЕТ: 5 САМЫХ ПОПУЛЯРНЫХ ТОВАРОВ\n";
        file << "========================================\n";
        file << "Период: " << start_date << " - " << end_date << "\n";
        file << "Дата формирования: " << end_date << "\n\n";
        
        if (result.empty()) {
            file << "Нет данных о продажах за указанный период.\n";
        } else {
            file << std::left << std::setw(5) << "№" 
                 << std::setw(30) << "Наименование товара" 
                 << std::setw(15) << "Количество" << "\n";
            file << "------------------------------------------------\n";
            
            int rank = 1;
            for (const auto& item : result) {
                file << std::left << std::setw(5) << rank 
                     << std::setw(30) << item.first 
                     << std::setw(15) << item.second << "\n";
                rank++;
            }
        }
        
        file << "\n========================================\n";
        file.close();
        std::cout << "Top goods report generated: top_goods_report.txt" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error generating top goods report: " << e.what() << std::endl;
    }
}

void ReportGenerator::generateDemandReport() {
    try {
        // Получаем текущую дату для отчета
        time_t now = time(0);
        tm* localtm = localtime(&now);
        char end_date[11];
        strftime(end_date, sizeof(end_date), "%Y-%m-%d", localtm);
        
        // Берем данные за последние 30 дней
        tm start_tm = *localtm;
        start_tm.tm_mday -= 30;
        mktime(&start_tm);
        char start_date[11];
        strftime(start_date, sizeof(start_date), "%Y-%m-%d", &start_tm);
        
        // Получаем список товаров для выбора
        auto goods = DatabaseManager::getInstance().getAllGoods();
        if (goods.empty()) {
            std::cerr << "No goods found for demand report" << std::endl;
            return;
        }
        
        // Для примера берем первый товар
        int goodId = goods[0].id;
        std::string goodName = goods[0].name;
        
        auto result = DatabaseManager::getInstance().getDemandData(goodId, start_date, end_date);
        
        std::ofstream file("demand_report.txt");
        if (!file.is_open()) {
            std::cerr << "Cannot open file for writing" << std::endl;
            return;
        }
        
        file << "========================================\n";
        file << "          ОТЧЕТ: АНАЛИЗ СПРОСА\n";
        file << "========================================\n";
        file << "Товар: " << goodName << " (ID: " << goodId << ")\n";
        file << "Период: " << start_date << " - " << end_date << "\n";
        file << "Дата формирования: " << end_date << "\n\n";
        
        if (result.empty()) {
            file << "Нет данных о продажах данного товара за указанный период.\n";
        } else {
            file << std::left << std::setw(15) << "Дата" 
                 << std::setw(15) << "Количество" << "\n";
            file << "-------------------------------\n";
            
            int total = 0;
            for (const auto& item : result) {
                file << std::left << std::setw(15) << item.first 
                     << std::setw(15) << item.second << "\n";
                total += item.second;
            }
            
            file << "-------------------------------\n";
            file << "Итого за период: " << total << " ед.\n";
            
            // Простая статистика
            if (!result.empty()) {
                double average = static_cast<double>(total) / result.size();
                file << "Среднее в день: " << std::fixed << std::setprecision(2) << average << " ед.\n";
            }
        }
        
        file << "\n========================================\n";
        file.close();
        std::cout << "Demand report generated: demand_report.txt" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error generating demand report: " << e.what() << std::endl;
    }
}

void ReportGenerator::generateSalesReport() {
    try {
        // Получаем текущую дату для отчета
        time_t now = time(0);
        tm* localtm = localtime(&now);
        char end_date[11];
        strftime(end_date, sizeof(end_date), "%Y-%m-%d", localtm);
        
        // Берем данные за последние 7 дней
        tm start_tm = *localtm;
        start_tm.tm_mday -= 7;
        mktime(&start_tm);
        char start_date[11];
        strftime(start_date, sizeof(start_date), "%Y-%m-%d", &start_tm);
        
        auto sales = DatabaseManager::getInstance().getAllSales();
        
        std::ofstream file("sales_report.txt");
        if (!file.is_open()) {
            std::cerr << "Cannot open file for writing" << std::endl;
            return;
        }
        
        file << "========================================\n";
        file << "          ОТЧЕТ ПО ПРОДАЖАМ\n";
        file << "========================================\n";
        file << "Период: " << start_date << " - " << end_date << "\n";
        file << "Дата формирования: " << end_date << "\n\n";
        
        if (sales.empty()) {
            file << "Нет данных о продажах за указанный период.\n";
        } else {
            file << std::left << std::setw(8) << "ID" 
                 << std::setw(25) << "Товар" 
                 << std::setw(12) << "Количество" 
                 << std::setw(12) << "Дата" << "\n";
            file << "----------------------------------------------------\n";
            
            int totalQuantity = 0;
            for (const auto& sale : sales) {
                // Фильтруем по дате
                if (sale.create_date >= start_date && sale.create_date <= end_date) {
                    file << std::left << std::setw(8) << sale.id 
                         << std::setw(25) << sale.good_name 
                         << std::setw(12) << sale.good_count 
                         << std::setw(12) << sale.create_date << "\n";
                    totalQuantity += sale.good_count;
                }
            }
            
            file << "----------------------------------------------------\n";
            file << "Всего продаж: " << totalQuantity << " ед.\n";
            file << "Количество транзакций: " << sales.size() << "\n";
        }
        
        file << "\n========================================\n";
        file.close();
        std::cout << "Sales report generated: sales_report.txt" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error generating sales report: " << e.what() << std::endl;
    }
}

void ReportGenerator::generateWarehouseReport() {
    try {
        auto stocks = DatabaseManager::getInstance().getWarehouseData();
        
        std::ofstream file("warehouse_report.txt");
        if (!file.is_open()) {
            std::cerr << "Cannot open file for writing" << std::endl;
            return;
        }
        
        // Получаем текущую дату
        time_t now = time(0);
        tm* localtm = localtime(&now);
        char current_date[11];
        strftime(current_date, sizeof(current_date), "%Y-%m-%d", localtm);
        
        file << "========================================\n";
        file << "          ОТЧЕТ ПО СКЛАДАМ\n";
        file << "========================================\n";
        file << "Дата формирования: " << current_date << "\n\n";
        
        if (stocks.empty()) {
            file << "Нет данных о складах.\n";
        } else {
            file << std::left << std::setw(25) << "Товар" 
                 << std::setw(12) << "Склад 1" 
                 << std::setw(12) << "Склад 2" 
                 << std::setw(18) << "Общее количество" << "\n";
            file << "------------------------------------------------------------\n";
            
            int totalWarehouse1 = 0;
            int totalWarehouse2 = 0;
            int grandTotal = 0;
            
            for (const auto& stock : stocks) {
                file << std::left << std::setw(25) << stock.good_name 
                     << std::setw(12) << stock.warehouse1_count 
                     << std::setw(12) << stock.warehouse2_count 
                     << std::setw(18) << stock.total_count << "\n";
                
                totalWarehouse1 += stock.warehouse1_count;
                totalWarehouse2 += stock.warehouse2_count;
                grandTotal += stock.total_count;
            }
            
            file << "------------------------------------------------------------\n";
            file << "Итого:\n";
            file << "Склад 1: " << totalWarehouse1 << " ед.\n";
            file << "Склад 2: " << totalWarehouse2 << " ед.\n";
            file << "Общее количество: " << grandTotal << " ед.\n";
        }
        
        file << "\n========================================\n";
        file.close();
        std::cout << "Warehouse report generated: warehouse_report.txt" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error generating warehouse report: " << e.what() << std::endl;
    }
}
