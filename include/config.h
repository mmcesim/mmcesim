/**
 * @file config.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Read and Write User Configuration
 * @version 0.3.0
 * @date 2024-01-21
 *
 * @copyright Copyright (c) 2022-2024 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "log_global.h"
#include "term.h"
#include "utils.h"
#include "yaml.h"
#include <array>
#include <exception>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <string>

static std::array valid_configs = { "latex"s, "cpp"s, "cppflags"s };

/**
 * @brief User configuration.
 *
 * @details Provide read and write portal.
 */
class Config {
  public:
    /**
     * @brief Construct a new Config object
     *
     * This is a default constructor.
     */
    Config();

    /**
     * @brief Read the key.
     *
     * @exception A std::runtime_error will be thrown if the key cannot be correctly accessed.
     * @param key The key.
     * @return (std::string) The value.
     */
    std::string operator[](const std::string& key) const;

    /**
     * @brief Set the key value.
     *
     * @details It will only set the key value when the key exists. Return false if the key does not exist.
     * @exception A std::runtime_error will be thrown if the key exists but the value cannot be set.
     * @param key The key.
     * @param v The value.
     * @retval true Successfully set the key.
     * @retval false Failed to set the key.
     */
    bool set(const std::string& key, const std::string& v);

    size_t size() const noexcept;

    bool write() const;

    static std::string read(const std::string& key, std::string* msg = nullptr);

    static std::string read(const std::string& key, const std::string& default_val, std::string* msg = nullptr);

    static bool edit(const std::string& key, const std::string& v, std::string* msg = nullptr);

  private:
    bool _load();

    YAML::Node _config;
};

inline size_t Config::size() const noexcept { return _config.size(); }

#endif
