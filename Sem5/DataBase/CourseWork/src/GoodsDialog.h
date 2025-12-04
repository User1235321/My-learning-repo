#ifndef GOODSDIALOG_H
#define GOODSDIALOG_H

#include <QDialog>

class QLineEdit;
class QSpinBox;
class QTableView;
class QStandardItemModel;
class QPushButton;

class GoodsDialog : public QDialog {
    Q_OBJECT

public:
    GoodsDialog(QWidget* parent = nullptr);

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void setupUI();
    void loadGoods();
    
    QLineEdit* nameEdit;
    QSpinBox* prioritySpinBox;
    QTableView* goodsTable;
    QStandardItemModel* model;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* closeButton;
};

#endif
