#include "SalesDialog.h"
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
#include <QInputDialog>
#include <QDialog>
#include <QModelIndexList>
#include <QDebug>

SalesDialog::SalesDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Управление заявками");
    setMinimumSize(700, 500);
    
    setupUI();
    loadGoods();
    loadSales();
}

void SalesDialog::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Форма ввода
    QHBoxLayout* formLayout = new QHBoxLayout();
    formLayout->addWidget(new QLabel("Товар:"));
    goodsComboBox = new QComboBox();
    formLayout->addWidget(goodsComboBox);
    
    formLayout->addWidget(new QLabel("Количество:"));
    countSpinBox = new QSpinBox();
    countSpinBox->setRange(1, 10000);
    countSpinBox->setValue(1);
    formLayout->addWidget(countSpinBox);
    
    addButton = new QPushButton("Добавить");
    formLayout->addWidget(addButton);
    
    layout->addLayout(formLayout);
    
    // Таблица
    model = new QStandardItemModel(0, 5, this);
    model->setHorizontalHeaderLabels({"ID", "Товар", "ID товара", "Количество", "Дата"});
    
    salesTable = new QTableView();
    salesTable->setModel(model);
    salesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    salesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    salesTable->horizontalHeader()->setStretchLastSection(true);
    salesTable->setColumnHidden(2, true);
    layout->addWidget(salesTable);
    
    // Кнопки управления
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    editButton = new QPushButton("Изменить");
    deleteButton = new QPushButton("Удалить");
    closeButton = new QPushButton("Закрыть");
    
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);
    
    // Подключение сигналов
    connect(addButton, &QPushButton::clicked, this, &SalesDialog::onAddClicked);
    connect(editButton, &QPushButton::clicked, this, &SalesDialog::onEditClicked);
    connect(deleteButton, &QPushButton::clicked, this, &SalesDialog::onDeleteClicked);
    connect(closeButton, &QPushButton::clicked, this, &SalesDialog::accept);
}

void SalesDialog::loadGoods() {
    goodsComboBox->clear();
    
    auto goods = DatabaseManager::getInstance().getAllGoods();
    for (const auto& good : goods) {
        goodsComboBox->addItem(QString::fromStdString(good.name), good.id);
    }
}

void SalesDialog::loadSales() {
    model->removeRows(0, model->rowCount());
    
    auto sales = DatabaseManager::getInstance().getAllSales();
    for (const auto& sale : sales) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(sale.id));
        items << new QStandardItem(QString::fromStdString(sale.good_name));
        items << new QStandardItem(QString::number(sale.good_id));
        items << new QStandardItem(QString::number(sale.good_count));
        items << new QStandardItem(QString::fromStdString(sale.create_date));
        model->appendRow(items);
    }
}

void SalesDialog::onAddClicked() {
    int goodId = goodsComboBox->currentData().toInt();
    int count = countSpinBox->value();
    
    if (goodId == 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар");
        return;
    }
    
    if (count <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите количество");
        return;
    }
    
    // Получаем доступное количество товара
    int availableStock = DatabaseManager::getInstance().getTotalStockForGood(goodId);
    
    if (count > availableStock) {
        QString goodName = goodsComboBox->currentText();
        QMessageBox::warning(this, "Ошибка", 
            QString("Недостаточно товара на складе!\n\n"
                    "Товар: %1\n"
                    "Запрошено: %2 шт.\n"
                    "Доступно на складах: %3 шт.\n"
                    "Не хватает: %4 шт.")
                .arg(goodName)
                .arg(count)
                .arg(availableStock)
                .arg(count - availableStock));
        return;
    }
    
    if (DatabaseManager::getInstance().addSale(goodId, count)) {
        QMessageBox::information(this, "Успех", "Заявка добавлена");
        countSpinBox->setValue(1);
        loadSales();
    } else {
        // Проверяем причину ошибки
        if (count > DatabaseManager::getInstance().getTotalStockForGood(goodId)) {
            int availableStock = DatabaseManager::getInstance().getTotalStockForGood(goodId);
            QString goodName = goodsComboBox->currentText();
            QMessageBox::warning(this, "Ошибка", 
                QString("Недостаточно товара на складе!\n\n"
                        "Товар: %1\n"
                        "Запрошено: %2 шт.\n"
                        "Доступно на складах: %3 шт.\n"
                        "Не хватает: %4 шт.")
                    .arg(goodName)
                    .arg(count)
                    .arg(availableStock)
                    .arg(count - availableStock));
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось добавить заявку");
        }
    }
}

void SalesDialog::onEditClicked() {
    QModelIndexList selected = salesTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите заявку для редактирования");
        return;
    }
    
    int row = selected.first().row();
    int id = model->item(row, 0)->text().toInt();
    int currentGoodId = model->item(row, 2)->text().toInt();
    int currentCount = model->item(row, 3)->text().toInt();
    
    // Диалог редактирования
    QDialog editDialog(this);
    editDialog.setWindowTitle("Редактирование заявки");
    QVBoxLayout layout(&editDialog);
    
    QHBoxLayout goodLayout;
    goodLayout.addWidget(new QLabel("Товар:"));
    QComboBox goodCombo;
    for (int i = 0; i < goodsComboBox->count(); ++i) {
        goodCombo.addItem(goodsComboBox->itemText(i), goodsComboBox->itemData(i));
    }
    goodCombo.setCurrentIndex(goodCombo.findData(currentGoodId));
    goodLayout.addWidget(&goodCombo);
    layout.addLayout(&goodLayout);
    
    QHBoxLayout countLayout;
    countLayout.addWidget(new QLabel("Количество:"));
    QSpinBox countSpin;
    countSpin.setRange(1, 10000);
    countSpin.setValue(currentCount);
    countLayout.addWidget(&countSpin);
    layout.addLayout(&countLayout);
    
    QHBoxLayout buttonLayout;
    QPushButton saveButton("Сохранить");
    QPushButton cancelButton("Отмена");
    buttonLayout.addWidget(&saveButton);
    buttonLayout.addWidget(&cancelButton);
    layout.addLayout(&buttonLayout);
    
    connect(&saveButton, &QPushButton::clicked, &editDialog, &QDialog::accept);
    connect(&cancelButton, &QPushButton::clicked, &editDialog, &QDialog::reject);
    
    if (editDialog.exec() == QDialog::Accepted) {
        int newGoodId = goodCombo.currentData().toInt();
        int newCount = countSpin.value();
        
        // Получаем доступное количество товара для нового количества
        int availableStock = DatabaseManager::getInstance().getTotalStockForGood(newGoodId);
        
        // Для обновления нужно учитывать, сколько уже было в заявке
        int difference = newCount;
        if (newGoodId == currentGoodId) {
            // Если товар тот же, то разница - это новое количество минус старое
            difference = newCount - currentCount;
            if (difference > availableStock) {
                QString goodName = goodCombo.currentText();
                QMessageBox::warning(&editDialog, "Ошибка", 
                    QString("Недостаточно товара на складе для увеличения заявки!\n\n"
                            "Товар: %1\n"
                            "Требуется дополнительно: %2 шт.\n"
                            "Доступно на складах: %3 шт.")
                        .arg(goodName)
                        .arg(difference)
                        .arg(availableStock));
                return;
            }
        } else {
            // Если товар другой, то нужно проверить полное новое количество
            if (newCount > availableStock) {
                QString goodName = goodCombo.currentText();
                QMessageBox::warning(&editDialog, "Ошибка", 
                    QString("Недостаточно товара на складе!\n\n"
                            "Товар: %1\n"
                            "Запрошено: %2 шт.\n"
                            "Доступно на складах: %3 шт.")
                        .arg(goodName)
                        .arg(newCount)
                        .arg(availableStock));
                return;
            }
        }
        
        if (DatabaseManager::getInstance().updateSale(id, newGoodId, newCount)) {
            QMessageBox::information(this, "Успех", "Заявка обновлена");
            loadSales();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось обновить заявку");
        }
    }
}

void SalesDialog::onDeleteClicked() {
    QModelIndexList selected = salesTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите заявку для удаления");
        return;
    }
    
    int row = selected.first().row();
    int id = model->item(row, 0)->text().toInt();
    QString goodName = model->item(row, 1)->text();
    int count = model->item(row, 3)->text().toInt();
    
    if (QMessageBox::question(this, "Подтверждение", 
        QString("Вы уверены, что хотите удалить заявку на товар \"%1\" в количестве %2?").arg(goodName).arg(count)) == QMessageBox::Yes) {
        
        if (DatabaseManager::getInstance().deleteSale(id)) {
            QMessageBox::information(this, "Успех", "Заявка удалена");
            loadSales();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить заявку");
        }
    }
}
