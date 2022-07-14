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

#include <iostream>
#include <fstream>
#include <filesystem>
#include <type_traits>
#include <ctime>
#include <boost/algorithm/string.hpp>
#include "export/keywords.h"
#include "error_code.h"
#include "read.h"
#include "cli_options.h"
#include "utils.h"

class Export {
public:
    /**
     * @brief Export language.
     * 
     * Options are CPP, MATLAB, PYTHON and IPYNB.
     */
    enum class Lang {
        CPP,    ///< C++ with Armadillo library
        MATLAB, ///< MATLAB
        OCTAVE, ///< GNU Octave
        PY,     ///< Python with NumPy library
        IPYNB   ///< IPyNb with NumPy library
    } lang = Lang::CPP;

    enum DType: unsigned {
        INT = 1,     ///< int
        DOUBLE = 2,  ///< double
        STRING = 4,  ///< string
        BOOL = 8,    ///< bool
        CHAR = 16,   ///< map
        SEQ = 32,    ///< sequence
        MAP = 64,    ///< map
        NUL = 1024,  ///< null
        UNDEF = 2048 ///< undefined
    };

    Export(const CLI_Options& opt);

    Export(const CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors);

    ~Export();

    YAML_Errors exportCode();

    static YAML_Errors exportCode(const CLI_Options& opt);

    static YAML_Errors exportCode(const CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors);

private:
    std::ofstream& _f();

    std::string _langName() const;

    std::string _langExtension() const;

    std::string _langHeaderExtension() const;

    std::string _langCommentSymbol() const;

    void _info(const std::string& str) const;

    // error message can be specified later
    bool _preCheck(const YAML::Node& node, unsigned allowed_type, bool mattered = true);

    template<typename T>
    T _as(const YAML::Node& n, bool mattered = true);

    std::string _asStr(const YAML::Node& n, bool mattered = true);

    void _setLatestError(const std::string& str);

    void _setLang();

    bool _isKeyword(const std::string& str) const;

    std::string _asVarName(const std::string& str) const;

    std::ofstream& _wComment();

    void _topComment();

    CLI_Options _opt;
    YAML::Node _config;
    YAML_Errors _errors;
    bool _already_error_before_export = false;
    std::ofstream* _f_ptr;
    
};

#endif
