#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTabWidget;
class QTableView;
class QStandardItemModel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(bool isAdmin = true, QWidget* parent = nullptr);

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
    
    bool adminMode;
    QTabWidget* tabWidget;
    QTableView* goodsTable;
    QTableView* salesTable;
    QTableView* warehouseTable;
    QStandardItemModel* goodsModel;
    QStandardItemModel* salesModel;
    QStandardItemModel* warehouseModel;
};

#endif
