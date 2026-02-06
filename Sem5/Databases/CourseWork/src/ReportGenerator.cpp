#include "ReportGenerator.h"
#include "DatabaseManager.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

void ReportGenerator::saveToFile(const std::string& filename, const std::string& content) {
    try {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "Report saved to: " << filename << std::endl;
        } else {
            std::cerr << "Cannot open file for writing: " << filename << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
    }
}

std::string ReportGenerator::generateTopGoodsReportContent() {
    std::stringstream ss;
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
        
        ss << "========================================\n";
        ss << "     ОТЧЕТ: 5 САМЫХ ПОПУЛЯРНЫХ ТОВАРОВ\n";
        ss << "========================================\n";
        ss << "Период: " << start_date << " - " << end_date << "\n";
        ss << "Дата формирования: " << end_date << "\n\n";
        
        if (result.empty()) {
            ss << "Нет данных о продажах за указанный период.\n";
        } else {
            ss << std::left << std::setw(5) << "№" 
               << std::setw(30) << "Наименование товара" 
               << std::setw(15) << "Количество" << "\n";
            ss << "------------------------------------------------\n";
            
            int rank = 1;
            for (const auto& item : result) {
                ss << std::left << std::setw(5) << rank 
                   << std::setw(30) << item.first 
                   << std::setw(15) << item.second << "\n";
                rank++;
            }
        }
        
        ss << "\n========================================\n";
    } catch (const std::exception& e) {
        ss << "Ошибка при формировании отчёта: " << e.what() << std::endl;
    }
    return ss.str();
}

std::string ReportGenerator::generateDemandReportContent() {
    std::stringstream ss;
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
            ss << "Нет товаров для анализа спроса.\n";
            return ss.str();
        }
        
        // Для примера берем первый товар
        int goodId = goods[0].id;
        std::string goodName = goods[0].name;
        
        auto result = DatabaseManager::getInstance().getDemandData(goodId, start_date, end_date);
        
        ss << "========================================\n";
        ss << "          ОТЧЕТ: АНАЛИЗ СПРОСА\n";
        ss << "========================================\n";
        ss << "Товар: " << goodName << " (ID: " << goodId << ")\n";
        ss << "Период: " << start_date << " - " << end_date << "\n";
        ss << "Дата формирования: " << end_date << "\n\n";
        
        if (result.empty()) {
            ss << "Нет данных о продажах данного товара за указанный период.\n";
        } else {
            ss << std::left << std::setw(15) << "Дата" 
               << std::setw(15) << "Количество" << "\n";
            ss << "-------------------------------\n";
            
            int total = 0;
            for (const auto& item : result) {
                ss << std::left << std::setw(15) << item.first 
                   << std::setw(15) << item.second << "\n";
                total += item.second;
            }
            
            ss << "-------------------------------\n";
            ss << "Итого за период: " << total << " ед.\n";
            
            // Простая статистика
            if (!result.empty()) {
                double average = static_cast<double>(total) / result.size();
                ss << "Среднее в день: " << std::fixed << std::setprecision(2) << average << " ед.\n";
            }
        }
        
        ss << "\n========================================\n";
    } catch (const std::exception& e) {
        ss << "Ошибка при формировании отчёта: " << e.what() << std::endl;
    }
    return ss.str();
}

std::string ReportGenerator::generateSalesReportContent() {
    std::stringstream ss;
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
        
        ss << "========================================\n";
        ss << "          ОТЧЕТ ПО ПРОДАЖАМ\n";
        ss << "========================================\n";
        ss << "Период: " << start_date << " - " << end_date << "\n";
        ss << "Дата формирования: " << end_date << "\n\n";
        
        if (sales.empty()) {
            ss << "Нет данных о продажах за указанный период.\n";
        } else {
            ss << std::left << std::setw(8) << "ID" 
               << std::setw(25) << "Товар" 
               << std::setw(12) << "Количество" 
               << std::setw(12) << "Дата" << "\n";
            ss << "----------------------------------------------------\n";
            
            int totalQuantity = 0;
            for (const auto& sale : sales) {
                // Фильтруем по дате
                if (sale.create_date >= start_date && sale.create_date <= end_date) {
                    ss << std::left << std::setw(8) << sale.id 
                       << std::setw(25) << sale.good_name 
                       << std::setw(12) << sale.good_count 
                       << std::setw(12) << sale.create_date << "\n";
                    totalQuantity += sale.good_count;
                }
            }
            
            ss << "----------------------------------------------------\n";
            ss << "Всего продаж: " << totalQuantity << " ед.\n";
            ss << "Количество транзакций: " << sales.size() << "\n";
        }
        
        ss << "\n========================================\n";
    } catch (const std::exception& e) {
        ss << "Ошибка при формировании отчёта: " << e.what() << std::endl;
    }
    return ss.str();
}

std::string ReportGenerator::generateWarehouseReportContent() {
    std::stringstream ss;
    try {
        auto stocks = DatabaseManager::getInstance().getWarehouseData();
        
        // Получаем текущую дату
        time_t now = time(0);
        tm* localtm = localtime(&now);
        char current_date[11];
        strftime(current_date, sizeof(current_date), "%Y-%m-%d", localtm);
        
        ss << "========================================\n";
        ss << "          ОТЧЕТ ПО СКЛАДАМ\n";
        ss << "========================================\n";
        ss << "Дата формирования: " << current_date << "\n\n";
        
        if (stocks.empty()) {
            ss << "Нет данных о складах.\n";
        } else {
            ss << std::left << std::setw(25) << "Товар" 
               << std::setw(12) << "Склад 1" 
               << std::setw(12) << "Склад 2" 
               << std::setw(18) << "Общее количество" << "\n";
            ss << "------------------------------------------------------------\n";
            
            int totalWarehouse1 = 0;
            int totalWarehouse2 = 0;
            int grandTotal = 0;
            
            for (const auto& stock : stocks) {
                ss << std::left << std::setw(25) << stock.good_name 
                   << std::setw(12) << stock.warehouse1_count 
                   << std::setw(12) << stock.warehouse2_count 
                   << std::setw(18) << stock.total_count << "\n";
                
                totalWarehouse1 += stock.warehouse1_count;
                totalWarehouse2 += stock.warehouse2_count;
                grandTotal += stock.total_count;
            }
            
            ss << "------------------------------------------------------------\n";
            ss << "Итого:\n";
            ss << "Склад 1: " << totalWarehouse1 << " ед.\n";
            ss << "Склад 2: " << totalWarehouse2 << " ед.\n";
            ss << "Общее количество: " << grandTotal << " ед.\n";
        }
        
        ss << "\n========================================\n";
    } catch (const std::exception& e) {
        ss << "Ошибка при формировании отчёта: " << e.what() << std::endl;
    }
    return ss.str();
}

// Старые методы для обратной совместимости
void ReportGenerator::generateTopGoodsReport() {
    std::string content = generateTopGoodsReportContent();
    saveToFile("top_goods_report.txt", content);
}

void ReportGenerator::generateDemandReport() {
    std::string content = generateDemandReportContent();
    saveToFile("demand_report.txt", content);
}

void ReportGenerator::generateSalesReport() {
    std::string content = generateSalesReportContent();
    saveToFile("sales_report.txt", content);
}

void ReportGenerator::generateWarehouseReport() {
    std::string content = generateWarehouseReportContent();
    saveToFile("warehouse_report.txt", content);
}

// Новые методы для получения отчета в виде строки
std::string ReportGenerator::getTopGoodsReport() {
    return generateTopGoodsReportContent();
}

std::string ReportGenerator::getDemandReport() {
    return generateDemandReportContent();
}

std::string ReportGenerator::getSalesReport() {
    return generateSalesReportContent();
}

std::string ReportGenerator::getWarehouseReport() {
    return generateWarehouseReportContent();
}
