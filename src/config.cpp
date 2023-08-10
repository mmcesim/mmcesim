/**
 * @file config.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Config Class
 * @version 0.2.2
 * @date 2023-08-07
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "config.h"

Config::Config() { _load(); }

std::string Config::operator[](const std::string& key) const noexcept {
    if (_config[key].IsDefined()) {
        try {
            return _config[key].as<std::string>();
        } catch (...) { return ""; }
    } else return "";
}

bool Config::set(const std::string& key, const std::string& v) noexcept {
    if (contains(valid_configs, key)) {
        try {
            _config[key] = v;
            return true;
        } catch (...) { return false; }
    } else return false;
}

bool Config::write() const {
    std::ofstream f(dataDir() + "/mmcesim.cfg");
    if (f.is_open()) {
        try {
            f << _config;
            return true;
        } catch (...) { return false; }
    } else return false;
}

std::string Config::read(const std::string& key) {
    Config c;
    return c[key];
}

std::string Config::read(const std::string& key, const std::string& default_val) {
    Config c;
    std::string val = c[key];
    return val != "" ? val : default_val;
}

bool Config::edit(const std::string& key, const std::string& v, std::string* msg) {
    Config c;
    if (c.set(key, v)) {
        if (c.write()) {
            return true;
        } else {
            if (msg) *msg = "Cannot write to configuration file.";
            return false;
        }
    } else {
        if (msg) *msg = "Key '" + key + "' is not a valid configuration option.";
        return false;
    }
}

bool Config::_load() {
    try {
        _config = YAML::LoadFile(dataDir() + "/mmcesim.cfg");
        return true;
    } catch (...) { return false; }
}
