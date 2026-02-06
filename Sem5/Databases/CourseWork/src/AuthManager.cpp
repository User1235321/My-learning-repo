#include "AuthManager.h"
#include <QCryptographicHash>
#include <QByteArray>
#include <QString>
#include <iostream>

// Инициализация статических переменных
std::string AuthManager::adminHash_;
std::string AuthManager::userHash_;
bool AuthManager::hashesLoaded_ = false;

std::string AuthManager::hashPassword(const std::string& password) {
    QByteArray hash = QCryptographicHash::hash(
        QByteArray::fromStdString(password), 
        QCryptographicHash::Sha256
    );
    return hash.toHex().toStdString();
}

bool AuthManager::verifyPassword(const std::string& password, const std::string& hash) {
    std::string hashedPassword = hashPassword(password);
    return hashedPassword == hash;
}

void AuthManager::loadHashesFromConfig(const std::string& adminHash, const std::string& userHash) {
    adminHash_ = adminHash;
    userHash_ = userHash;
    hashesLoaded_ = true;
    std::cout << "Hashes loaded from config" << std::endl;
}

bool AuthManager::authenticateUser(const std::string& username, const std::string& password) {
    std::cout << "Debug: Trying to login with username: '" << username << "'" << std::endl;
    
    if (!hashesLoaded_) {
        std::cerr << "Error: Password hashes not loaded from config!" << std::endl;
        return false;
    }
    
    if (username == "admin") {
        if (verifyPassword(password, adminHash_)) {
            std::cout << "Debug: Admin login successful" << std::endl;
            return true;
        }
    } else if (username == "user") {
        if (verifyPassword(password, userHash_)) {
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
