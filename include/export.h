/**
 * @file export.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Export mmCEsim Configuration to Other Programming Languages
 * @version 0.1.0
 * @date 2022-07-12
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _EXPORT_H_
#define _EXPORT_H_

#include "read.h"

class Export {
public:
    /**
     * @brief Export language.
     * 
     * Options are CPP, MATLAB, PYTHON and IPYNB.
     */
    enum class Lang {
        CPP,    ///< C++ with Armadillo library
        MATLAB, ///< Matlab or Octave
        PY,     ///< Python with NumPy library
        IPYNB   ///< IPyNb with NumPy library
    } lang;

    Export(const std::string& file);

    Export(const YAML::Node& config, const YAML_Errors& errors);

    YAML_Errors exportCode();

    static YAML_Errors exportCode(const std::string& file);

    static YAML_Errors exportCode(const YAML::Node& config, const YAML_Errors& errors);

private:
    void _setLang();

    YAML::Node _config;
    YAML_Errors _errors;
    
};

#endif
