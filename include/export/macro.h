/**
 * @file macro.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief ALG Macro
 * @version 0.2.2
 * @date 2024-01-15
 *
 * @copyright Copyright (c) 2022-2024 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_MACRO_H_
#define _EXPORT_MACRO_H_

#include "export/lang.h"
#include "log_global.h"
#include "meta.h"
#include "utils.h"
#include <map>
#include <regex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct XY_Size {
    XY_Size() = default;

    unsigned Tx = 0;
    unsigned Ty = 0;
    unsigned Rx = 0;
    unsigned Ry = 0;

    unsigned t() const noexcept { return Tx * Ty; }
    unsigned r() const noexcept { return Rx * Ry; }
    unsigned _() const noexcept { return t() * r(); }

    std::map<std::string, std::pair<unsigned, unsigned>> nodes_xy;

    bool hasKey(std::string id) const noexcept { return nodes_xy.count(id); }

    std::pair<unsigned, unsigned> xy(std::string id) const {
        try {
            return nodes_xy.at(id); // with boundary check
        } catch (const std::out_of_range& e) {
            // TODO: better error handling
            _log.err() << "No '" + id + "' node when replacing macro (`SIZE[<id>]`, `BEAM[<id>]`, `GRID[<id>]`)!"
                       << std::endl;
            Term::error("No '" + id + "' node when replacing macro (`SIZE[<id>]`, `BEAM[<id>]`, `GRID[<id>]`)!");
            return { 0, 0 };
        }
    }

    unsigned x(std::string id) const { return xy(id).first; }
    unsigned y(std::string id) const { return xy(id).second; }
    unsigned x_mul_y(std::string id) const {
        auto&& x_and_y = this->xy(id);
        return x_and_y.first * x_and_y.second;
    }
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
    Lang lang = Lang::CPP;

    bool replaceXY(std::string& r) const;

    std::string replaceMacro(const std::string& s, int job_cnt, int alg_cnt) const;

  private:
    enum class Type {
        PRE,
        USER,
        USER_PRIORITY,
    };

    // TODO: This only applies to C++ only now, and needs to be extended.
    const std::map<std::string, std::pair<std::string, bool>> _constants = {
        { R"(CHS\.PATHS_NUM)", { "CHS_paths_num", false } },
        { R"(CHS\.CHANNELS\[(.*\w+.*)\])", { "CHS_channels[$1]", true } },
        { R"(CHS\.CHANNELS\[(.*\w+.*)\]\.ID)", { "CHS_channels_id[$1]", false } },
        { R"(CHS\.NODES\[(.*\w+.*)\]\.ID)", { "CHS_nodes_id[$1]", false } },
        { R"(CHS\.FROM\[(.*\w+.*)\])", { "CHS_from[$1]", false } },
        { R"(CHS\.FROM\[(.*\w+.*)\]\.INDEX)", { "CHS_from[$1]", false } },
        { R"(CHS\.FROM\[(.*\w+.*)\]\.ID)", { "CHS_nodes_id[" + constantStr({ "CHS_from[$1]", false }) + "]", false } },
        { R"(CHS\.TO\[(.*\w+.*)\])", { "CHS_to[$1]", false } },
        { R"(CHS\.TO\[(.*\w+.*)\]\.INDEX)", { "CHS_to[$1]", false } },
        { R"(CHS\.TO\[(.*\w+.*)\]\.ID)", { "CHS_nodes_id[" + constantStr({ "CHS_to[$1]", false }) + "]", false } },
        { R"(CHS\[(.*\w+.*)\]\.JUMPS_NUM)", { "CHS_i_jumps_num[$1]", false } },
        { R"(CHS\[(.*\w+.*)\]\.SIZE)", { "CHS_i_size[$1]", false } },
        { R"(CHS\.ALL_CHANNELS\[(.*\w+.*)\]\.INDEX)", { "CHS_all_channels_index[$1]", false } },
        { R"(CHS\.ALL_CHANNELS\[(.*\w+.*)\])",
          { "CHS_channels[" + constantStr({ "CHS_all_channels_index[$1]", false }) + "]", true } },
        { R"(CHS\.ALL_CHANNELS\[(.*\w+.*)\]\.ID)",
          { "CHS_channels_id[" + constantStr({ "CHS_all_channels_index[$1]", false }) + "]", false } },
        { R"(CHS\.NUM_CHANNELS_ACC\[(.*\w+.*)\])", { "CHS_num_channels_acc[$1]", false } },
        { R"(CHS\[(.*\w+.*)\]\[(.*\w+.*)\])",
          { "CHS_channels[" + constantStr({ "CHS_all_channels_index[CHS_num_channels_acc[$1] + ($2)]", false }) + "]",
            true } },
        { R"(CHS\[(.*\w+.*)\]\[(.*\w+.*)\]\.ID)",
          { "CHS_channels_id[" + constantStr({ "CHS_all_channels_index[CHS_num_channels_acc[$1] + ($2)]", false }) +
                "]",
            false } },
        { R"(CHS\[(.*\w+.*)\]\[(.*\w+.*)\]\.FROM)",
          { "CHS_from[" + constantStr({ "CHS_all_channels_index[CHS_num_channels_acc[$1] + ($2)]", false }) + "]",
            false } },
        { R"(CHS\[(.*\w+.*)\]\[(.*\w+.*)\]\.FROM\.INDEX)",
          { "CHS_from[" + constantStr({ "CHS_all_channels_index[CHS_num_channels_acc[$1] + ($2)]", false }) + "]",
            false } },
        { R"(CHS\[(.*\w+.*)\]\[(.*\w+.*)\]\.FROM\.ID)",
          { "CHS_nodes_id[" +
                constantStr({ "CHS_from[" +
                                  constantStr({ "CHS_all_channels_index[CHS_num_channels_acc[$1] + ($2)]", false }) +
                                  "]",
                              false }) +
                "]",
            false } },
        { R"(CHS\[(.*\w+.*)\]\[(.*\w+.*)\]\.TO)",
          { "CHS_to[" + constantStr({ "CHS_all_channels_index[CHS_num_channels_acc[$1] + ($2)]", false }) + "]",
            false } },
        { R"(CHS\[(.*\w+.*)\]\[(.*\w+.*)\]\.TO\.INDEX)",
          { "CHS_to[" + constantStr({ "CHS_all_channels_index[CHS_num_channels_acc[$1] + ($2)]", false }) + "]",
            false } },
        { R"(CHS\[(.*\w+.*)\]\[(.*\w+.*)\]\.TO\.ID)",
          { "CHS_nodes_id[" +
                constantStr({ "CHS_to[" +
                                  constantStr({ "CHS_all_channels_index[CHS_num_channels_acc[$1] + ($2)]", false }) +
                                  "]",
                              false }) +
                "]",
            false } },
    };

    /**
     * @brief Get the name of Type.
     *
     * @param type The macro type (predefined, user defined or user defined with priority).
     * @return (std::string) The name of Type.
     */
    std::string typeName(Type type) const noexcept;

    std::string constantStr(const std::pair<std::string, bool>& v) const;

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

inline std::string Macro::constantStr(const std::pair<std::string, bool>& v) const {
    if (v.first.empty()) return "";
    if (lang == Lang::CPP) {
        if (v.second) return "(*mmCEsim_Consts_"s + v.first + ")";
        else return "mmCEsim_Consts_"s + v.first;
    } else return v.first;
}

#endif
