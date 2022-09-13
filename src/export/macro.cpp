/**
 * @file macro.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Macro class.
 * @version 0.1.0
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include "export/macro.h"

std::string Macro::replaceMacro(const std::string& s, int cnt) const {
    std::regex alg_num_re("`ALG_NUM`");
    std::regex alg_name_re("`ALG_NAME`");
    std::regex alg_params_re("`ALG_PARAMS`");
    std::string r = std::regex_replace(s, alg_num_re, std::to_string(this->alg_num));
    if (cnt >= 0) {
        r = std::regex_replace(r, alg_name_re, this->alg_names[cnt]);
        r = std::regex_replace(r, alg_params_re, this->alg_params[cnt]);
    }
    return r;
}
