#include "AuthManager.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QString>
#include <iostream>

std::string AuthManager::hashPassword(const std::string& password) {
    // Используем SHA256 для хеширования пароля
    QByteArray hash = QCryptographicHash::hash(
        QByteArray::fromStdString(password), 
        QCryptographicHash::Sha256
    );
    return hash.toHex().toStdString();
}

bool AuthManager::verifyPassword(const std::string& password, const std::string& hash) {
    // Хешируем введенный пароль и сравниваем с хранимым хешем
    std::string hashedPassword = hashPassword(password);
    return hashedPassword == hash;
}

bool AuthManager::authenticateUser(const std::string& username, const std::string& password) {
    std::cout << "Debug: Trying to login with username: '" << username << "'" << std::endl;
    
    // Для демонстрации - статические хеши
    // В реальном приложении эти хеши должны браться из базы данных или конфига
    std::string adminHash = "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918"; // admin
    std::string userHash = "04f8996da763b7a969b1028ee3007569eaf3a635486ddab211d512c85b9df8fb"; // user123
    
    if (username == "admin") {
        if (verifyPassword(password, adminHash)) {
            std::cout << "Debug: Admin login successful" << std::endl;
            return true;
        }
    } else if (username == "user") {
        if (verifyPassword(password, userHash)) {
            std::cout << "Debug: User login successful" << std::endl;
            return true;
        }
    }
    
    std::cout << "Debug: Login failed" << std::endl;
    return false;
}

bool AuthManager::isAdmin(const std::string& username) {
    return username == "admin";
}
