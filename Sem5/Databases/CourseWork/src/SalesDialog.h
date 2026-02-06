#ifndef SALESDIALOG_H
#define SALESDIALOG_H

#include <QDialog>

class QComboBox;
class QSpinBox;
class QTableView;
class QStandardItemModel;
class QPushButton;

class SalesDialog : public QDialog {
    Q_OBJECT

public:
    SalesDialog(QWidget* parent = nullptr);

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void setupUI();
    void loadGoods();
    void loadSales();
    
    QComboBox* goodsComboBox;
    QSpinBox* countSpinBox;
    QTableView* salesTable;
    QStandardItemModel* model;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* closeButton;
};

#endif
