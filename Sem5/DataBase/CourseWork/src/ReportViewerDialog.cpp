#include "ReportViewerDialog.h"
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QDateTime>
#include <QLabel>

ReportViewerDialog::ReportViewerDialog(const QString& reportTitle, const QString& reportContent, QWidget* parent)
    : QDialog(parent), reportTitle(reportTitle), reportContent(reportContent) {
    setWindowTitle(reportTitle);
    setMinimumSize(800, 600);
    
    setupUI();
}

void ReportViewerDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Заголовок
    QLabel* titleLabel = new QLabel(reportTitle);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 14pt; font-weight: bold; margin: 10px; }");
    mainLayout->addWidget(titleLabel);
    
    // Текстовое поле с отчетом
    textEdit = new QTextEdit();
    textEdit->setPlainText(reportContent);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Courier New", 10)); // Моноширинный шрифт для таблиц
    mainLayout->addWidget(textEdit);
    
    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    saveButton = new QPushButton("Сохранить в файл");
    printButton = new QPushButton("Печать");
    closeButton = new QPushButton("Закрыть");
    
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(printButton);
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Подключение сигналов
    connect(saveButton, &QPushButton::clicked, this, &ReportViewerDialog::onSaveClicked);
    connect(printButton, &QPushButton::clicked, this, &ReportViewerDialog::onPrintClicked);
    connect(closeButton, &QPushButton::clicked, this, &ReportViewerDialog::accept);
}

void ReportViewerDialog::onSaveClicked() {
    QString fileName = QFileDialog::getSaveFileName(this, 
        "Сохранить отчет", 
        reportTitle.replace(":", "_").replace(" ", "_") + ".txt",
        "Текстовые файлы (*.txt);;Все файлы (*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << reportContent;
        file.close();
        QMessageBox::information(this, "Сохранение", "Отчет успешно сохранен в файл.");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
    }
}

void ReportViewerDialog::onPrintClicked() {
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);
    
    if (printDialog.exec() == QDialog::Accepted) {
        textEdit->print(&printer);
    }
}
