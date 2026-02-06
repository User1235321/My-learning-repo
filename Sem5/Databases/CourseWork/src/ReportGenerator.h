#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <string>

class ReportGenerator {
public:
    // Старые методы для сохранения в файлы
    static void generateTopGoodsReport();
    static void generateDemandReport();
    static void generateSalesReport();
    static void generateWarehouseReport();
    
    // Новые методы для получения отчета в виде строки
    static std::string getTopGoodsReport();
    static std::string getDemandReport();
    static std::string getSalesReport();
    static std::string getWarehouseReport();
    
private:
    static void saveToFile(const std::string& filename, const std::string& content);
    static std::string generateTopGoodsReportContent();
    static std::string generateDemandReportContent();
    static std::string generateSalesReportContent();
    static std::string generateWarehouseReportContent();
};

#endif
