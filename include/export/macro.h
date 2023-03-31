/**
 * @file macro.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief ALG Macro
 * @version 0.2.1
 * @date 2023-03-17
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_MACRO_H_
#define _EXPORT_MACRO_H_

#include "log_global.h"
#include "meta.h"
#include "utils.h"
#include <map>
#include <regex>
#include <string>
#include <utility>
#include <vector>

struct XY_Size {
    unsigned Tx = 0;
    unsigned Ty = 0;
    unsigned Rx = 0;
    unsigned Ry = 0;

    unsigned t() const { return Tx * Ty; }
    unsigned r() const { return Rx * Ry; }
    unsigned _() const { return t() * r(); }
};

struct Macro {
    unsigned job_num = 0;
    std::vector<unsigned> alg_num;
    std::vector<std::vector<std::string>> alg_names;
    std::vector<std::vector<std::string>> alg_params;
    std::vector<std::vector<std::vector<std::pair<std::string, std::string>>>> alg_custom;
    std::map<std::string, std::string> custom_priority;
    std::map<std::string, std::string> custom;
    std::map<std::string, std::string> custom_in_alg;
    std::map<std::string, std::string> beamforming;
    std::string _cascaded_channel;
    XY_Size _N, _B, _G;

    std::string replaceMacro(const std::string& s, int job_cnt, int alg_cnt) const;

  private:
    enum class Type {
        PRE,
        USER,
        USER_PRIORITY,
    };

    /**
     * @brief Get the name of Type.
     *
     * @param type The macro type (predefined, user defined or user defined with priority).
     * @return (std::string) The name of Type.
     */
    std::string typeName(Type type) const noexcept;

  public:
    /**
     * @brief RegEx replace.
     *
     * @details During replacing, log information will be written.
     * @param s The original string.
     * @param key The key to be replaced.
     * @param val The value to replace the key.
     * @return (std::string) The replaced string.
     */
    std::string replace(const std::string& s, const std::string& key, const std::string& val,
                        Type type = Type::PRE) const;
};

static const Macro macro_none;

inline std::string Macro::typeName(Type type) const noexcept {
    return type == Type::PRE ? "Predefined" : type == Type::USER ? "User" : "User Priority";
}

inline std::string Macro::replace(const std::string& s, const std::string& key, const std::string& val,
                                  Type type) const {
    auto r = std::regex_replace(s, std::regex("`" + key + "`"), val);
    if (r != s) _log.info() << "Macro Replace (" << typeName(type) << "): " << key << " -> " << val << std::endl;
    return r;
}

#endif
