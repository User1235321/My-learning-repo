#include <QApplication>
#include <QMessageBox>
#include "LoginDialog.h"
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "ConfigManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Загрузка конфигурации
    ConfigManager config("config.ini");
    if (!config.load()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось загрузить конфигурационный файл");
        return 1;
    }
    
    // Подключение к БД
    std::string host = config.getValue("Database", "Host");
    std::string port = config.getValue("Database", "Port");
    std::string dbname = config.getValue("Database", "DatabaseName");
    std::string user = config.getValue("Database", "User");
    std::string password = config.getValue("Database", "Password");
    
    if (!DatabaseManager::getInstance().connect(host, port, dbname, user, password)) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось подключиться к базе данных");
        return 1;
    }
    
    // Авторизация
    LoginDialog loginDialog;
    if (loginDialog.exec() != QDialog::Accepted) {
        return 0;
    }
    
    // Запуск главного окна
    MainWindow mainWindow(loginDialog.isAdmin());
    mainWindow.show();
    
    return app.exec();
}
