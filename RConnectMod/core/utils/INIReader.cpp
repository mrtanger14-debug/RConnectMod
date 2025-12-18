#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

class INIReader {

public:

    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line, currentSection;
        while (std::getline(file, line)) {
            if (line.find(';') != std::string::npos) line = line.substr(0, line.find(';'));
            if (line.find('#') != std::string::npos) line = line.substr(0, line.find('#'));
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            if (line.empty()) continue;
            if (line.front() == '[' && line.back() == ']') {
                currentSection = line.substr(1, line.size() - 2);
            } else {
                auto eqPos = line.find('=');
                if (eqPos == std::string::npos) continue;
                std::string key = line.substr(0, eqPos);
                std::string value = line.substr(eqPos + 1);
                key.erase(0, key.find_first_not_of(" \t\r\n"));
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);
                data[currentSection][key] = value;
            }
        }
        return true;
    }

    std::string get(const std::string& section, const std::string& key, const std::string& defaultValue = "") const {
        auto secIt = data.find(section);
        if (secIt != data.end()) {
            auto keyIt = secIt->second.find(key);
            if (keyIt != secIt->second.end())
                return keyIt->second;
        }
        return defaultValue;
    }

private:

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

};
