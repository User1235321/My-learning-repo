#include "WarehouseDialog.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItemModel>

WarehouseDialog::WarehouseDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Управление складами");
    setMinimumSize(700, 500);
    
    setupUI();
    loadGoods();
    loadWarehouse();
}

void WarehouseDialog::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Форма ввода
    QHBoxLayout* formLayout = new QHBoxLayout();
    formLayout->addWidget(new QLabel("Товар:"));
    goodsComboBox = new QComboBox();
    formLayout->addWidget(goodsComboBox);
    
    formLayout->addWidget(new QLabel("Склад 1:"));
    count1SpinBox = new QSpinBox();
    count1SpinBox->setRange(0, 100000);
    formLayout->addWidget(count1SpinBox);
    
    formLayout->addWidget(new QLabel("Склад 2:"));
    count2SpinBox = new QSpinBox();
    count2SpinBox->setRange(0, 100000);
    formLayout->addWidget(count2SpinBox);
    
    updateButton = new QPushButton("Обновить");
    formLayout->addWidget(updateButton);
    
    layout->addLayout(formLayout);
    
    // Таблица
    model = new QStandardItemModel(0, 4, this);
    model->setHorizontalHeaderLabels({"Товар", "Склад 1", "Склад 2", "Общее количество"});
    
    warehouseTable = new QTableView();
    warehouseTable->setModel(model);
    warehouseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    warehouseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    warehouseTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(warehouseTable);
    
    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    closeButton = new QPushButton("Закрыть");
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);
    
    // Подключение сигналов
    connect(updateButton, &QPushButton::clicked, this, &WarehouseDialog::onUpdateClicked);
    connect(closeButton, &QPushButton::clicked, this, &WarehouseDialog::accept);
    connect(goodsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &WarehouseDialog::onGoodsSelectionChanged);
}

void WarehouseDialog::loadGoods() {
    goodsComboBox->clear();
    
    auto goods = DatabaseManager::getInstance().getAllGoods();
    for (const auto& good : goods) {
        goodsComboBox->addItem(QString::fromStdString(good.name), good.id);
    }
}

void WarehouseDialog::loadWarehouse() {
    model->removeRows(0, model->rowCount());
    
    auto stocks = DatabaseManager::getInstance().getWarehouseData();
    for (const auto& stock : stocks) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::fromStdString(stock.good_name));
        items << new QStandardItem(QString::number(stock.warehouse1_count));
        items << new QStandardItem(QString::number(stock.warehouse2_count));
        items << new QStandardItem(QString::number(stock.total_count));
        model->appendRow(items);
    }
}

void WarehouseDialog::onUpdateClicked() {
    int goodId = goodsComboBox->currentData().toInt();
    int count1 = count1SpinBox->value();
    int count2 = count2SpinBox->value();
    
    if (goodId == 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар");
        return;
    }
    
    if (DatabaseManager::getInstance().updateWarehouse1(goodId, count1) &&
        DatabaseManager::getInstance().updateWarehouse2(goodId, count2)) {
        QMessageBox::information(this, "Успех", "Данные складов обновлены");
        loadWarehouse();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось обновить данные складов");
    }
}

void WarehouseDialog::onGoodsSelectionChanged() {
    int goodId = goodsComboBox->currentData().toInt();
    if (goodId == 0) return;
    
    auto stock = DatabaseManager::getInstance().getWarehouseStock(goodId);
    count1SpinBox->setValue(stock.first);
    count2SpinBox->setValue(stock.second);
}
