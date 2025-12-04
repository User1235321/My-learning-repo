#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <string>

class AuthManager {
public:
    static std::string hashPassword(const std::string& password);
    static bool verifyPassword(const std::string& password, const std::string& hash);
    static bool authenticateUser(const std::string& username, const std::string& password);
    static bool isAdmin(const std::string& username);
};

#endif
