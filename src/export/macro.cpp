/**
 * @file macro.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Macro class.
 * @version 0.2.1
 * @date 2023-05-05
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "export/macro.h"
#include <iostream>

std::string Macro::replaceMacro(const std::string& s, int job_cnt, int alg_cnt) const {
    std::string r = replace(s, "JOB_NUM", std::to_string(this->job_num));
    r             = replace(r, "JOB_CNT", std::to_string(job_cnt));
    r             = replace(r, "NMSE", "NMSE" + std::to_string(job_cnt));
    r             = replace(r, "PILOT", "pilot");
    r             = replace(r, "SNR_dB", "SNR_dB");
    r             = replace(r, "CARRIERS_NUM", "carriers_num");
    if (_N.Tx > 0) r = replace(r, "SIZE\\.T\\.x", std::to_string(_N.Tx));
    if (_N.Ty > 0) r = replace(r, "SIZE\\.T\\.y", std::to_string(_N.Ty));
    if (_N.Rx > 0) r = replace(r, "SIZE\\.R\\.x", std::to_string(_N.Rx));
    if (_N.Ry > 0) r = replace(r, "SIZE\\.R\\.y", std::to_string(_N.Ry));
    if (_N.t() > 0) r = replace(r, "SIZE\\.T", std::to_string(_N.t()));
    if (_N.r() > 0) r = replace(r, "SIZE\\.R", std::to_string(_N.r()));
    if (_N._() > 0) r = replace(r, "SIZE\\.\\*", std::to_string(_N._()));
    if (_B.Tx > 0) r = replace(r, "BEAM\\.T\\.x", std::to_string(_B.Tx));
    if (_B.Ty > 0) r = replace(r, "BEAM\\.T\\.y", std::to_string(_B.Ty));
    if (_B.Rx > 0) r = replace(r, "BEAM\\.R\\.x", std::to_string(_B.Rx));
    if (_B.Ry > 0) r = replace(r, "BEAM\\.R\\.y", std::to_string(_B.Ry));
    if (_B.t() > 0) r = replace(r, "BEAM\\.T", std::to_string(_B.t()));
    if (_B.r() > 0) r = replace(r, "BEAM\\.R", std::to_string(_B.r()));
    if (_B._() > 0) r = replace(r, "BEAM\\.\\*", std::to_string(_B._()));
    if (_G.Tx > 0) r = replace(r, "GRID\\.T\\.x", std::to_string(_G.Tx));
    if (_G.Ty > 0) r = replace(r, "GRID\\.T\\.y", std::to_string(_G.Ty));
    if (_G.Rx > 0) r = replace(r, "GRID\\.R\\.x", std::to_string(_G.Rx));
    if (_G.Ry > 0) r = replace(r, "GRID\\.R\\.y", std::to_string(_G.Ry));
    if (_G.t() > 0) r = replace(r, "GRID\\.T", std::to_string(_G.t()));
    if (_G.r() > 0) r = replace(r, "GRID\\.R", std::to_string(_G.r()));
    if (_G._() > 0) r = replace(r, "GRID\\.\\*", std::to_string(_G._()));
    if (_N.t() > 0) {
        r = replace(r, "DICTIONARY.T",
                    "\\dictionary(" + std::to_string(_N.Tx) + "," + std::to_string(_N.Ty) + "," +
                        std::to_string(_G.Tx) + "," + std::to_string(_G.Ty) + ")");
    }
    if (_N.r() > 0) {
        r = replace(r, "DICTIONARY.R",
                    "\\dictionary(" + std::to_string(_N.Rx) + "," + std::to_string(_N.Ry) + "," +
                        std::to_string(_G.Rx) + "," + std::to_string(_G.Ry) + ")");
    }
    if (_B.r() > 0) r = replace(r, "MEASUREMENT", "(pilot*" + std::to_string(_B.r()) + ")");
    for (const auto& [key, val] : custom_priority) { r = replace(r, key, val, Type::USER_PRIORITY); }
    if (job_cnt >= 0) {
        r = replace(r, "ALG_NUM", std::to_string(this->alg_num[job_cnt]));
        if (alg_cnt >= 0) {
            r = replace(r, "ALG_NAME", this->alg_names[job_cnt][alg_cnt]);
            r = replace(r, "ALG_PARAMS", this->alg_params[job_cnt][alg_cnt]);
            if (alg_custom.size() > job_cnt && alg_custom[job_cnt].size() > alg_cnt) {
                auto&& pairs = alg_custom[job_cnt][alg_cnt];
                for (const auto& [key, val] : pairs) r = replace(r, key, val, Type::USER);
            }
            for (const auto& [key, val] : custom_in_alg) r = replace(r, key, val, Type::USER);
        }
    }
    r = replace(r, "VERSION", std::to_string(_MMCESIM_VER));
    r = replace(r, "CAS_CH", _cascaded_channel);
    for (const auto& [key, val] : _constants) r = replace(r, key, constantStr(val));
    for (const auto& [key, val] : beamforming) r = replace(r, "BF\\[" + key + "\\]", val);
    for (const auto& [key, val] : custom) r = replace(r, key, val, Type::USER);
    _log.flush();
    return r;
}
