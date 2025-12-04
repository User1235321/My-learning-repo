#include "GoodsDialog.h"
#include "DatabaseManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QModelIndexList>
#include <QModelIndex>

GoodsDialog::GoodsDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Управление товарами");
    setMinimumSize(600, 400);
    
    setupUI();
    loadGoods();
}

void GoodsDialog::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Форма ввода
    QHBoxLayout* formLayout = new QHBoxLayout();
    formLayout->addWidget(new QLabel("Наименование:"));
    nameEdit = new QLineEdit();
    formLayout->addWidget(nameEdit);
    
    formLayout->addWidget(new QLabel("Приоритет:"));
    prioritySpinBox = new QSpinBox();
    prioritySpinBox->setRange(1, 100);
    prioritySpinBox->setValue(1);
    formLayout->addWidget(prioritySpinBox);
    
    addButton = new QPushButton("Добавить");
    formLayout->addWidget(addButton);
    
    layout->addLayout(formLayout);
    
    // Таблица
    model = new QStandardItemModel(0, 3, this);
    model->setHorizontalHeaderLabels({"ID", "Наименование", "Приоритет"});
    
    goodsTable = new QTableView();
    goodsTable->setModel(model);
    goodsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    goodsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    goodsTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(goodsTable);
    
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
    connect(addButton, &QPushButton::clicked, this, &GoodsDialog::onAddClicked);
    connect(editButton, &QPushButton::clicked, this, &GoodsDialog::onEditClicked);
    connect(deleteButton, &QPushButton::clicked, this, &GoodsDialog::onDeleteClicked);
    connect(closeButton, &QPushButton::clicked, this, &GoodsDialog::accept);
}

void GoodsDialog::loadGoods() {
    model->removeRows(0, model->rowCount());
    
    auto goods = DatabaseManager::getInstance().getAllGoods();
    for (const auto& good : goods) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(good.id));
        items << new QStandardItem(QString::fromStdString(good.name));
        items << new QStandardItem(QString::number(good.priority));
        model->appendRow(items);
    }
}

void GoodsDialog::onAddClicked() {
    QString name = nameEdit->text();
    int priority = prioritySpinBox->value();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите наименование товара");
        return;
    }
    
    if (DatabaseManager::getInstance().addGood(name.toStdString(), priority)) {
        QMessageBox::information(this, "Успех", "Товар добавлен");
        nameEdit->clear();
        prioritySpinBox->setValue(1);
        loadGoods();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить товар");
    }
}

void GoodsDialog::onEditClicked() {
    QModelIndexList selected = goodsTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для редактирования");
        return;
    }
    
    int row = selected.first().row();
    int id = model->item(row, 0)->text().toInt();
    QString name = model->item(row, 1)->text();
    int priority = model->item(row, 2)->text().toInt();
    
    bool ok;
    QString newName = QInputDialog::getText(this, "Редактирование", "Наименование:", QLineEdit::Normal, name, &ok);
    if (!ok || newName.isEmpty()) return;
    
    int newPriority = QInputDialog::getInt(this, "Редактирование", "Приоритет:", priority, 1, 100, 1, &ok);
    if (!ok) return;
    
    if (DatabaseManager::getInstance().updateGood(id, newName.toStdString(), newPriority)) {
        QMessageBox::information(this, "Успех", "Товар обновлен");
        loadGoods();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось обновить товар");
    }
}

void GoodsDialog::onDeleteClicked() {
    QModelIndexList selected = goodsTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для удаления");
        return;
    }
    
    int row = selected.first().row();
    int id = model->item(row, 0)->text().toInt();
    QString name = model->item(row, 1)->text();
    
    if (QMessageBox::question(this, "Подтверждение", 
        QString("Вы уверены, что хотите удалить товар \"%1\"?").arg(name)) == QMessageBox::Yes) {
        
        if (DatabaseManager::getInstance().deleteGood(id)) {
            QMessageBox::information(this, "Успех", "Товар удален");
            loadGoods();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить товар");
        }
    }
}
