#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <map>

class ConfigManager {
private:
    std::map<std::string, std::map<std::string, std::string>> data_;
    std::string filename_;

public:
    ConfigManager(const std::string& filename);
    bool load();
    std::string getValue(const std::string& section, const std::string& key, const std::string& default_value = "");
    void setValue(const std::string& section, const std::string& key, const std::string& value);
    bool save();
};

#endif
