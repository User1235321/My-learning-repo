#include "ConfigManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

ConfigManager::ConfigManager(const std::string& filename) : filename_(filename) {}

bool ConfigManager::load() {
    std::ifstream file(filename_);
    if (!file.is_open()) {
        std::cerr << "Cannot open config file: " << filename_ << std::endl;
        return false;
    }
    
    std::string line;
    std::string current_section;
    
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty() || line[0] == ';' || line[0] == '#') {
            continue;
        }
        
        if (line[0] == '[' && line[line.size()-1] == ']') {
            current_section = line.substr(1, line.size() - 2);
            continue;
        }
        
        size_t equals_pos = line.find('=');
        if (equals_pos != std::string::npos) {
            std::string key = line.substr(0, equals_pos);
            std::string value = line.substr(equals_pos + 1);
            
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            data_[current_section][key] = value;
        }
    }
    
    file.close();
    return true;
}

std::string ConfigManager::getValue(const std::string& section, const std::string& key, const std::string& default_value) {
    auto section_it = data_.find(section);
    if (section_it != data_.end()) {
        auto key_it = section_it->second.find(key);
        if (key_it != section_it->second.end()) {
            return key_it->second;
        }
    }
    return default_value;
}

void ConfigManager::setValue(const std::string& section, const std::string& key, const std::string& value) {
    data_[section][key] = value;
}

bool ConfigManager::save() {
    std::ofstream file(filename_);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& section : data_) {
        file << "[" << section.first << "]\n";
        for (const auto& key_value : section.second) {
            file << key_value.first << "=" << key_value.second << "\n";
        }
        file << "\n";
    }
    
    file.close();
    return true;
}
