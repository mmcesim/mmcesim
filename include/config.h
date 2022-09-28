/**
 * @file config.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Read and Write User Configuration
 * @version 0.1.0
 * @date 2022-09-28
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <vector>
#include <fstream>
#include "yaml.h"
#include "utils.h"

static std::vector<std::string> valid_configs = {
    "latex",
    "cpp",
    "cppflags"
};

class Config {
public:

    Config();

    std::string operator[](const std::string& key) const noexcept;

    bool set(const std::string& key, const std::string& v) noexcept;

    size_t size() const noexcept;

    bool write() const;

    static std::string read(const std::string& key);

    static bool edit(const std::string& key, const std::string& v, std::string* msg);

private:
    bool _load();

    YAML::Node _config;
};

inline size_t Config::size() const noexcept {
    return _config.size();
}

#endif
