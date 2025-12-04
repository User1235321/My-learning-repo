#ifndef WAREHOUSEDIALOG_H
#define WAREHOUSEDIALOG_H

#include <QDialog>

class QComboBox;
class QSpinBox;
class QTableView;
class QStandardItemModel;
class QPushButton;

class WarehouseDialog : public QDialog {
    Q_OBJECT

public:
    WarehouseDialog(QWidget* parent = nullptr);

private slots:
    void onUpdateClicked();
    void onGoodsSelectionChanged();

private:
    void setupUI();
    void loadGoods();
    void loadWarehouse();
    
    QComboBox* goodsComboBox;
    QSpinBox* count1SpinBox;
    QSpinBox* count2SpinBox;
    QTableView* warehouseTable;
    QStandardItemModel* model;
    QPushButton* updateButton;
    QPushButton* closeButton;
};

#endif
