/**
 * @file config.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Config Class
 * @version 0.2.2
 * @date 2024-01-21
 *
 * @copyright Copyright (c) 2022-2024 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "config.h"

Config::Config() { _load(); }

std::string Config::operator[](const std::string& key) const {
    try {
        if (_config[key].IsDefined()) {
            try {
                return _config[key].as<std::string>();
            } catch (...) { return ""; }
        } else return "";
    } catch (std::exception& e) {
        auto cfg_file = dataDir() + "/mmcesim.cfg";
        Term::error(
            fmt::format("Cannot read key '{}'. Possible reason: corrupt configuration file '{}'.", key, cfg_file));
        // The following error message will be added later when the exception is caught.
        // Term::error("Check the log for more information.");
        _log.err() << "Cannot read key '" << key << "'. Possible reason: corrupt configuration file.\n";
        _log.err() << "Contents of the configuration file:\n";
        _log.err() << _config << std::endl;
        _log.err() << "Exception message:\n";
        _log.err() << e.what() << std::endl;
        throw std::runtime_error("Cannot read key '" + key + "'. Possible reason: corrupt configuration file.");
    }
}

bool Config::set(const std::string& key, const std::string& v) {
    if (contains(valid_configs, key)) {
        try {
            _config[key] = v;
            return true;
        } catch (std::exception& e) {
            auto cfg_file = dataDir() + "/mmcesim.cfg";
            Term::error(fmt::format("Cannot set key '{}' as '{}'. Possible reason: corrupt configuration file '{}'.",
                                    key, v, cfg_file));
            // The following error message will be added later when the exception is caught.
            // Term::error("Check the log for more information.");
            _log.err() << "Cannot set key '" << key << "' as '" << v
                       << "'. Possible reason: corrupt configuration file.\n";
            _log.err() << "Contents of the configuration file:\n";
            _log.err() << _config << std::endl;
            _log.err() << "Exception message:\n";
            _log.err() << e.what() << std::endl;
            throw std::runtime_error("Cannot set key '" + key + "' as '" + v +
                                     "'. Possible reason: corrupt configuration file.");
            return false;
        }
    } else return false;
}

bool Config::write() const {
    std::ofstream f(dataDir() + "/mmcesim.cfg");
    if (f.is_open()) {
        try {
            f << _config << std::endl;
            f.close();
            return true;
        } catch (std::exception&) {
            f.close();
            return false;
        }
    } else return false;
}

std::string Config::read(const std::string& key, std::string* msg) {
    Config c;
    try {
        return c[key];
    } catch (std::runtime_error) {
        // error already logged
        if (msg) *msg = "Check the log for more information.";
        return "";
    }
}

std::string Config::read(const std::string& key, const std::string& default_val, std::string* msg) {
    std::string val = read(key, msg);
    return val.empty() ? default_val : val;
}

bool Config::edit(const std::string& key, const std::string& v, std::string* msg) {
    Config c;
    try {
        if (c.set(key, v)) { // exception can be thrown here (set).
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
    } catch (std::runtime_error) {
        // error already logged
        if (msg) *msg = "Check the log for more information.";
        return false;
    }
}

bool Config::_load() {
    std::string cfg_file = dataDir() + "/mmcesim.cfg";
    if (std::filesystem::exists(cfg_file)) {
        try {
            // Why I am doing this is because I do not know why suddenly on my MacBook,
            // the loaded YAML file always contain an addition document with content 1.
            // So the loaded result it is like:
            // ```yml
            // 1
            // ---
            // cpp: clang
            // ```
            auto cfgs = YAML::LoadAllFromFile(cfg_file);
            _config   = cfgs.end()[-1];
            std::cout << cfg_file << std::endl;
            std::cout << "Loaded\n" << _config << std::endl;
            return true;
        } catch (std::exception& e) {
            // TODO: error handling cfg file on load
            Term::error("Error while loading configuration file.");
            _log.err() << "Error while loading configuration file." << std::endl;
            return false;
        }
    } else {
        // Nothing to do. A fresh start!
        return true;
    }
}
