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

std::string Macro::replaceMacro(const std::string& s, int job_cnt, int alg_cnt) const {
    std::regex job_num_re("`JOB_NUM`");
    std::regex alg_num_re("`ALG_NUM`");
    std::regex alg_name_re("`ALG_NAME`");
    std::regex alg_params_re("`ALG_PARAMS`");
    std::string r = std::regex_replace(s, job_num_re, std::to_string(this->job_num));
    if (job_cnt >= 0) {
        r = std::regex_replace(r, alg_num_re, std::to_string(this->alg_num[job_cnt]));
        if (alg_cnt >= 0) {
            r = std::regex_replace(r, alg_name_re, this->alg_names[job_cnt][alg_cnt]);
            r = std::regex_replace(r, alg_params_re, this->alg_params[job_cnt][alg_cnt]);
        } // else {
        //     if (lang == "cpp") {
        //         r = std::regex_replace(r, alg_name_re, "_macro_alg_names[" + std::to_string(alg_cnt) + "]");
        //         r = std::regex_replace(r, alg_params_re, "_macro_alg_params[" + std::to_string(alg_cnt) + "]");
        //     }
        // }
    }
    return r;
}
