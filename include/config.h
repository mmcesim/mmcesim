/**
 * @file config.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Read and Write User Configuration
 * @version 0.2.1
 * @date 2022-09-28
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "utils.h"
#include "yaml.h"
#include <fstream>
#include <string>
#include <vector>

static std::vector<std::string> valid_configs = { "latex", "cpp", "cppflags" };

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
     * @param key The key.
     * @return (std::string) The value.
     */
    std::string operator[](const std::string& key) const noexcept;

    /**
     * @brief Set the key value.
     *
     * @details It will only set the key value when the key exists. Return false if the key does not exist.
     *          No exception will be raised in this function,
     *          but it will return false if there is error when setting the key.
     * @param key The key.
     * @param v The value.
     * @retval true Successfully set the key.
     * @retval false Failed to set the key.
     */
    bool set(const std::string& key, const std::string& v) noexcept;

    size_t size() const noexcept;

    bool write() const;

    static std::string read(const std::string& key);

    static std::string read(const std::string& key, const std::string& default_val);

    static bool edit(const std::string& key, const std::string& v, std::string* msg);

  private:
    bool _load();

    YAML::Node _config;
};

inline size_t Config::size() const noexcept { return _config.size(); }

#endif
