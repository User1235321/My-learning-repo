#include "AuthManager.h"
#include <iostream>

std::string AuthManager::hashPassword(const std::string& password) {
    // Временно возвращаем пароль без хеширования для отладки
    return password;
}

bool AuthManager::verifyPassword(const std::string& password, const std::string& hash) {
    // Временно сравниваем без хеширования
    return password == hash;
}

bool AuthManager::authenticateUser(const std::string& username, const std::string& password) {
    std::cout << "Debug: Trying to login with username: '" << username 
              << "', password: '" << password << "'" << std::endl;
    
    // Временно используем простую проверку
    if (username == "admin" && password == "admin123") {
        std::cout << "Debug: Admin login successful" << std::endl;
        return true;
    }
    if (username == "user" && password == "user123") {
        std::cout << "Debug: User login successful" << std::endl;
        return true;
    }
    
    std::cout << "Debug: Login failed" << std::endl;
    return false;
}

bool AuthManager::isAdmin(const std::string& username) {
    return username == "admin";
}
