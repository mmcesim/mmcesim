/**
 * @file macro.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief ALG Macro
 * @version 0.1.0
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _EXPORT_MACRO_H_
#define _EXPORT_MACRO_H_

#include <string>
#include <vector>
#include <regex>
#include "utils.h"

struct Macro {
    unsigned alg_num = 0;
    std::vector<std::string> alg_names; // ALG names separated by comma (,)
    std::vector<std::string> alg_params;

    std::string replaceMacro(const std::string& s, int cnt) const;
};

static const Macro macro_none;

#endif