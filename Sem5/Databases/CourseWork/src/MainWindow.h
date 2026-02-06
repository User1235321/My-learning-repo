#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

class QTabWidget;
class QTableView;
class QStandardItemModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(const std::string& username, bool isAdmin = true, QWidget* parent = nullptr);

private slots:
    void showGoods();
    void showSales();
    void showWarehouse();
    void generateTopGoodsReport();
    void generateDemandReport();
    void generateSalesReport();
    void generateWarehouseReport();

private:
    void setupUI();
    void setupMenu();
    void loadData();
    void loadGoods();
    void loadSales();
    void loadWarehouse();
    void showReport(const QString& title, const QString& content);
    
    std::string username;
    bool adminMode;
    QTabWidget* tabWidget;
    QTableView* goodsTable;
    QTableView* salesTable;
    QTableView* warehouseTable;
    QStandardItemModel* goodsModel;
    QStandardItemModel* salesModel;
    QStandardItemModel* warehouseModel;
    QLabel* userLabel;
};

#endif
