/**
 * @file macro.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief ALG Macro
 * @version 0.1.0
 * @date 2023-01-05
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_MACRO_H_
#define _EXPORT_MACRO_H_

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
    std::map<std::string, std::string> custom;
    std::map<std::string, std::string> custom_in_alg;
    std::map<std::string, std::string> beamforming;
    std::string _cascaded_channel;
    XY_Size _N, _B, _G;

    std::string replaceMacro(const std::string& s, int job_cnt, int alg_cnt) const;
};

static const Macro macro_none;

#endif
