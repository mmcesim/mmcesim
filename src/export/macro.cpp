/**
 * @file macro.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Macro class.
 * @version 0.2.0
 * @date 2023-01-15
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "export/macro.h"
#include <iostream>

std::string Macro::replaceMacro(const std::string& s, int job_cnt, int alg_cnt) const {
    std::regex job_num_re("`JOB_NUM`");
    std::regex job_cnt_re("`JOB_CNT`");
    std::regex job_nmse_re("`NMSE`");
    std::regex alg_num_re("`ALG_NUM`");
    std::regex alg_name_re("`ALG_NAME`");
    std::regex alg_params_re("`ALG_PARAMS`");
    std::regex pilot_re("`PILOT`");
    std::regex snr_db_re("`SNR_dB`");
    std::regex measurement_re("`MEASUREMENT`");
    std::string r = std::regex_replace(s, job_num_re, std::to_string(this->job_num));
    r             = std::regex_replace(r, job_cnt_re, std::to_string(job_cnt));
    r             = std::regex_replace(r, job_nmse_re, "NMSE" + std::to_string(job_cnt));
    r             = std::regex_replace(r, pilot_re, "pilot");
    r             = std::regex_replace(r, snr_db_re, "SNR_dB");
    r             = std::regex_replace(r, std::regex("`CARRIERS_NUM`"), "carriers_num");
    if (_N.Tx > 0) r = std::regex_replace(r, std::regex("`SIZE.T.x`"), std::to_string(_N.Tx));
    if (_N.Ty > 0) r = std::regex_replace(r, std::regex("`SIZE.T.y`"), std::to_string(_N.Ty));
    if (_N.Rx > 0) r = std::regex_replace(r, std::regex("`SIZE.R.x`"), std::to_string(_N.Rx));
    if (_N.Ry > 0) r = std::regex_replace(r, std::regex("`SIZE.R.y`"), std::to_string(_N.Ry));
    if (_N.t() > 0) r = std::regex_replace(r, std::regex("`SIZE.T`"), std::to_string(_N.t()));
    if (_N.r() > 0) r = std::regex_replace(r, std::regex("`SIZE.R`"), std::to_string(_N.r()));
    if (_N._() > 0) r = std::regex_replace(r, std::regex("`SIZE.\\*`"), std::to_string(_N._()));
    if (_B.Tx > 0) r = std::regex_replace(r, std::regex("`BEAM.T.x`"), std::to_string(_B.Tx));
    if (_B.Ty > 0) r = std::regex_replace(r, std::regex("`BEAM.T.y`"), std::to_string(_B.Ty));
    if (_B.Rx > 0) r = std::regex_replace(r, std::regex("`BEAM.R.x`"), std::to_string(_B.Rx));
    if (_B.Ry > 0) r = std::regex_replace(r, std::regex("`BEAM.R.y`"), std::to_string(_B.Ry));
    if (_B.t() > 0) r = std::regex_replace(r, std::regex("`BEAM.T`"), std::to_string(_B.t()));
    if (_B.r() > 0) r = std::regex_replace(r, std::regex("`BEAM.R`"), std::to_string(_B.r()));
    if (_B._() > 0) r = std::regex_replace(r, std::regex("`BEAM.\\*`"), std::to_string(_B._()));
    if (_G.Tx > 0) r = std::regex_replace(r, std::regex("`GRID.T.x`"), std::to_string(_G.Tx));
    if (_G.Ty > 0) r = std::regex_replace(r, std::regex("`GRID.T.y`"), std::to_string(_G.Ty));
    if (_G.Rx > 0) r = std::regex_replace(r, std::regex("`GRID.R.x`"), std::to_string(_G.Rx));
    if (_G.Ry > 0) r = std::regex_replace(r, std::regex("`GRID.R.y`"), std::to_string(_G.Ry));
    if (_G.t() > 0) r = std::regex_replace(r, std::regex("`GRID.T`"), std::to_string(_G.t()));
    if (_G.r() > 0) r = std::regex_replace(r, std::regex("`GRID.R`"), std::to_string(_G.r()));
    if (_G._() > 0) r = std::regex_replace(r, std::regex("`GRID.\\*`"), std::to_string(_G._()));
    if (_N.t() > 0) {
        std::regex dictionary_x_re("`DICTIONARY.T`");
        r = std::regex_replace(r, dictionary_x_re,
                               "\\dictionary(" + std::to_string(_N.Tx) + "," + std::to_string(_N.Ty) + "," +
                                   std::to_string(_G.Tx) + "," + std::to_string(_G.Ty) + ")");
    }
    if (_N.r() > 0) {
        std::regex dictionary_x_re("`DICTIONARY.R`");
        r = std::regex_replace(r, dictionary_x_re,
                               "\\dictionary(" + std::to_string(_N.Rx) + "," + std::to_string(_N.Ry) + "," +
                                   std::to_string(_G.Rx) + "," + std::to_string(_G.Ry) + ")");
    }
    if (_B.r() > 0) r = std::regex_replace(r, measurement_re, "(pilot*" + std::to_string(_B.r()) + ")");
    for (const auto& [key, val] : custom_priority) {
        std::cout << "Replacing user's custom priority macro.\n";
        r = std::regex_replace(r, std::regex("`" + key + "`"), val);
    }
    if (job_cnt >= 0) {
        r = std::regex_replace(r, alg_num_re, std::to_string(this->alg_num[job_cnt]));
        if (alg_cnt >= 0) {
            r = std::regex_replace(r, alg_name_re, this->alg_names[job_cnt][alg_cnt]);
            r = std::regex_replace(r, alg_params_re, this->alg_params[job_cnt][alg_cnt]);
            if (alg_custom.size() > job_cnt && alg_custom[job_cnt].size() > alg_cnt) {
                auto&& pairs = alg_custom[job_cnt][alg_cnt];
                for (auto&& pair : pairs) {
                    std::cout << "Replacing alg custom macro.\n";
                    r = std::regex_replace(r, std::regex("`" + pair.first + "`"), pair.second);
                }
            }
            for (const auto& [key, val] : custom_in_alg) {
                std::cout << "Replacing user's in_alg custom macro.\n";
                r = std::regex_replace(r, std::regex("`" + key + "`"), val);
            }
        }
    }
    r = std::regex_replace(r, std::regex("`VERSION`"), std::to_string(_MMCESIM_VER));
    r = std::regex_replace(r, std::regex("`CAS_CH`"), _cascaded_channel);
    for (const auto& [key, val] : beamforming) r = std::regex_replace(r, std::regex("`BF\\[" + key + "\\]`"), val);
    for (const auto& [key, val] : custom) {
        std::cout << "Replacing user's custom macro.\n";
        r = std::regex_replace(r, std::regex("`" + key + "`"), val);
    }
    return r;
}
