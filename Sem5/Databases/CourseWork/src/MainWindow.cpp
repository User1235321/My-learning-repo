#include "MainWindow.h"
#include "DatabaseManager.h"
#include "GoodsDialog.h"
#include "SalesDialog.h"
#include "WarehouseDialog.h"
#include "ReportGenerator.h"
#include "ReportViewerDialog.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTabWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QStatusBar>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QHBoxLayout>

MainWindow::MainWindow(const std::string& username, bool isAdmin, QWidget* parent) 
    : QMainWindow(parent), username(username), adminMode(isAdmin) {
    
    setWindowTitle("Оптовая фирма - Система управления");
    setMinimumSize(1000, 600);
    
    setupUI();
    setupMenu();
    loadData();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Создаем панель с информацией о пользователе
    QHBoxLayout* infoLayout = new QHBoxLayout();
    
    userLabel = new QLabel();
    userLabel->setText(QString("Пользователь: %1").arg(QString::fromStdString(username)));
    userLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    userLabel->setStyleSheet("QLabel { padding: 5px; font-weight: bold; }");
    
    QLabel* modeLabel = new QLabel();
    modeLabel->setText(adminMode ? "Режим: Администратор" : "Режим: Пользователь");
    modeLabel->setStyleSheet("QLabel { padding: 5px; font-weight: bold; color: blue; }");
    
    infoLayout->addWidget(modeLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(userLabel);
    
    mainLayout->addLayout(infoLayout);
    
    tabWidget = new QTabWidget(this);
    
    // Вкладка товаров
    QWidget* goodsTab = new QWidget();
    QVBoxLayout* goodsLayout = new QVBoxLayout(goodsTab);
    goodsTable = new QTableView();
    goodsModel = new QStandardItemModel(0, 3, this);
    goodsModel->setHorizontalHeaderLabels({"ID", "Наименование", "Приоритет"});
    goodsTable->setModel(goodsModel);
    goodsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    goodsTable->horizontalHeader()->setStretchLastSection(true);
    goodsLayout->addWidget(goodsTable);
    tabWidget->addTab(goodsTab, "Товары");
    
    // Вкладка заявок
    QWidget* salesTab = new QWidget();
    QVBoxLayout* salesLayout = new QVBoxLayout(salesTab);
    salesTable = new QTableView();
    salesModel = new QStandardItemModel(0, 4, this);
    salesModel->setHorizontalHeaderLabels({"ID", "Товар", "Количество", "Дата"});
    salesTable->setModel(salesModel);
    salesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    salesTable->horizontalHeader()->setStretchLastSection(true);
    salesLayout->addWidget(salesTable);
    tabWidget->addTab(salesTab, "Заявки");
    
    // Вкладка складов
    QWidget* warehouseTab = new QWidget();
    QVBoxLayout* warehouseLayout = new QVBoxLayout(warehouseTab);
    warehouseTable = new QTableView();
    warehouseModel = new QStandardItemModel(0, 4, this);
    warehouseModel->setHorizontalHeaderLabels({"Товар", "Склад 1", "Склад 2", "Общее количество"});
    warehouseTable->setModel(warehouseModel);
    warehouseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    warehouseTable->horizontalHeader()->setStretchLastSection(true);
    warehouseLayout->addWidget(warehouseTable);
    tabWidget->addTab(warehouseTab, "Склады");
    
    mainLayout->addWidget(tabWidget);
    
    statusBar()->showMessage("Готово");
}

void MainWindow::setupMenu() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    QMenu* referencesMenu = menuBar->addMenu("Справочники");
    QAction* goodsAction = new QAction("Товары", this);
    QAction* warehouseAction = new QAction("Склады", this);
    referencesMenu->addAction(goodsAction);
    referencesMenu->addAction(warehouseAction);
    
    connect(goodsAction, &QAction::triggered, this, &MainWindow::showGoods);
    connect(warehouseAction, &QAction::triggered, this, &MainWindow::showWarehouse);
    
    QMenu* journalsMenu = menuBar->addMenu("Журналы");
    QAction* salesAction = new QAction("Заявки", this);
    journalsMenu->addAction(salesAction);
    
    connect(salesAction, &QAction::triggered, this, &MainWindow::showSales);
    
    QMenu* reportsMenu = menuBar->addMenu("Отчеты");
    QAction* topGoodsReportAction = new QAction("Топ товаров", this);
    QAction* demandReportAction = new QAction("Анализ спроса", this);
    QAction* salesReportAction = new QAction("Отчет по продажам", this);
    QAction* warehouseReportAction = new QAction("Отчет по складам", this);
    
    reportsMenu->addAction(topGoodsReportAction);
    reportsMenu->addAction(demandReportAction);
    reportsMenu->addAction(salesReportAction);
    reportsMenu->addAction(warehouseReportAction);
    
    connect(topGoodsReportAction, &QAction::triggered, this, &MainWindow::generateTopGoodsReport);
    connect(demandReportAction, &QAction::triggered, this, &MainWindow::generateDemandReport);
    connect(salesReportAction, &QAction::triggered, this, &MainWindow::generateSalesReport);
    connect(warehouseReportAction, &QAction::triggered, this, &MainWindow::generateWarehouseReport);
    
    if (!adminMode) {
        goodsAction->setEnabled(false);
        warehouseAction->setEnabled(false);
        salesReportAction->setEnabled(false);
        warehouseReportAction->setEnabled(false);
    }
}

void MainWindow::loadData() {
    loadGoods();
    loadSales();
    loadWarehouse();
}

void MainWindow::loadGoods() {
    goodsModel->removeRows(0, goodsModel->rowCount());
    
    auto goods = DatabaseManager::getInstance().getAllGoods();
    for (const auto& good : goods) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(good.id));
        items << new QStandardItem(QString::fromStdString(good.name));
        items << new QStandardItem(QString::number(good.priority));
        goodsModel->appendRow(items);
    }
}

void MainWindow::loadSales() {
    salesModel->removeRows(0, salesModel->rowCount());
    
    auto sales = DatabaseManager::getInstance().getAllSales();
    for (const auto& sale : sales) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(sale.id));
        items << new QStandardItem(QString::fromStdString(sale.good_name));
        items << new QStandardItem(QString::number(sale.good_count));
        items << new QStandardItem(QString::fromStdString(sale.create_date));
        salesModel->appendRow(items);
    }
}

void MainWindow::loadWarehouse() {
    warehouseModel->removeRows(0, warehouseModel->rowCount());
    
    auto stocks = DatabaseManager::getInstance().getWarehouseData();
    for (const auto& stock : stocks) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::fromStdString(stock.good_name));
        items << new QStandardItem(QString::number(stock.warehouse1_count));
        items << new QStandardItem(QString::number(stock.warehouse2_count));
        items << new QStandardItem(QString::number(stock.total_count));
        warehouseModel->appendRow(items);
    }
}

void MainWindow::showReport(const QString& title, const QString& content) {
    ReportViewerDialog dialog(title, content, this);
    dialog.exec();
}

void MainWindow::showGoods() {
    GoodsDialog dialog(this);
    dialog.exec();
    loadGoods();
}

void MainWindow::showSales() {
    SalesDialog dialog(this);
    dialog.exec();
    loadSales();
}

void MainWindow::showWarehouse() {
    WarehouseDialog dialog(this);
    dialog.exec();
    loadWarehouse();
}

void MainWindow::generateTopGoodsReport() {
    // Генерируем и сохраняем отчет в файл
    ReportGenerator::generateTopGoodsReport();
    
    // Получаем отчет в виде строки для отображения
    std::string reportStr = ReportGenerator::getTopGoodsReport();
    showReport("Отчет: 5 самых популярных товаров", QString::fromStdString(reportStr));
}

void MainWindow::generateDemandReport() {
    ReportGenerator::generateDemandReport();
    std::string reportStr = ReportGenerator::getDemandReport();
    showReport("Отчет: Анализ спроса", QString::fromStdString(reportStr));
}

void MainWindow::generateSalesReport() {
    ReportGenerator::generateSalesReport();
    std::string reportStr = ReportGenerator::getSalesReport();
    showReport("Отчет по продажам", QString::fromStdString(reportStr));
}

void MainWindow::generateWarehouseReport() {
    ReportGenerator::generateWarehouseReport();
    std::string reportStr = ReportGenerator::getWarehouseReport();
    showReport("Отчет по складам", QString::fromStdString(reportStr));
}
