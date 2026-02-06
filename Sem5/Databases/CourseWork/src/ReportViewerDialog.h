#ifndef REPORTVIEWERDIALOG_H
#define REPORTVIEWERDIALOG_H

#include <QDialog>

class QTextEdit;
class QPushButton;

class ReportViewerDialog : public QDialog {
    Q_OBJECT

public:
    explicit ReportViewerDialog(const QString& reportTitle, const QString& reportContent, QWidget* parent = nullptr);

private slots:
    void onSaveClicked();
    void onPrintClicked();

private:
    void setupUI();
    
    QString reportTitle;
    QString reportContent;
    QTextEdit* textEdit;
    QPushButton* saveButton;
    QPushButton* printButton;
    QPushButton* closeButton;
};

#endif
