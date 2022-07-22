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
#include <vector>
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

    Export(CLI_Options& opt);

    Export(CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors);

    ~Export();

    YAML_Errors exportCode();

    static YAML_Errors exportCode(CLI_Options& opt);

    static YAML_Errors exportCode(CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors);

private:
    std::ofstream& _f();

    std::string _langName() const;

    std::string _langExtension() const;

    std::string _langHeaderExtension() const;

    std::string _langMmcesimExtension() const;

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

    void _beginning();

    void _generateChannels();

    void _ending();

    bool _setTransmitterReceiver();

    CLI_Options& _opt;
    YAML::Node _config;
    YAML_Errors _errors;
    bool _already_error_before_export = false;
    std::ofstream* _f_ptr = nullptr;
    std::vector<int> _transmitters;
    std::vector<int> _receivers;

    const int _MAX_TX = 1;
    const int _MAX_RX = 1;
};

template<typename T>
inline T Export::_as(const YAML::Node& n, bool mattered) {
    bool l;
    if (std::is_same_v<T, int>) {
        l = _preCheck(n, DType::INT, mattered);
    } else if (std::is_same_v<T, double>) {
        l = _preCheck(n, DType::DOUBLE, mattered);
    } else if (std::is_same_v<T, std::string>) {
        l = _preCheck(n, DType::STRING, mattered);
    } else if (std::is_same_v<T, bool>) {
        l = _preCheck(n, DType::BOOL, mattered);
    } else if (std::is_same_v<T, char>) {
        l = _preCheck(n, DType::CHAR, mattered);
    }
    if (l) {
        return n.as<T>();
    } else {
        throw("Invalid!");
    }
}

inline std::ofstream& Export::_f() {
    return *_f_ptr;
}

inline std::string Export::_langName() const {
    if (lang == Lang::CPP) return "C++ (with Armadillo library)";
    else if (lang == Lang::MATLAB) return "MATLAB";
    else if (lang == Lang::OCTAVE) return "GNU Octave";
    else if (lang == Lang::PY) return "Python (with NumPy library)";
    else if (lang == Lang::IPYNB) return "IPyNb (with NumPy library)";
    else return "Impossible branch in \"Export::_langName()\"!";
}

inline std::string Export::_langExtension() const {
    if (lang == Lang::CPP) return "cpp";
    else if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return "m";
    else if (lang == Lang::PY) return "py";
    else if (lang == Lang::IPYNB) return "ipynb";
    else return "Impossible branch in \"Export::_langExtension()\"!";
}

inline std::string Export::_langHeaderExtension() const {
    if (lang == Lang::CPP) return "h";
    else if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return "m";
    else if (lang == Lang::PY) return "py";
    else if (lang == Lang::IPYNB) return "ipynb";
    else return "Impossible branch in \"Export::_langHeaderExtension()\"!";
}

inline std::string Export::_langMmcesimExtension() const {
    if (lang == Lang::CPP) return "mmcesim-cpp";
    else if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return "mmcesim-m";
    else if (lang == Lang::PY || lang == Lang::IPYNB) return "mmcesim-py";
    else return "Impossible branch in \"Export::_langMmcesimExtension()\"!";
}

inline std::string Export::_langCommentSymbol() const {
    if (lang == Lang::CPP) return "//";
    else if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return "%";
    else if (lang == Lang::PY || lang == Lang::IPYNB) return "#";
    else return "Impossible branch in \"Export::_langCommentSymbol()\"!";
}

inline void Export::_info(const std::string& str) const {
    std::cout << "[mmCEsim] export $ " << str << std::endl;
}

inline std::string Export::_asStr(const YAML::Node& n, bool mattered) {
    return _as<std::string>(n, mattered);
}

inline void Export::_setLatestError(const std::string& str) {
    assert((!_errors.empty() && "Check if errors are empty when trying to edit the last record."));
    (_errors.end() - 1)->msg = str;
}

inline bool Export::_isKeyword(const std::string& str) const {
    if (lang == Lang::CPP) return contains(CPP_Keywords, str);
    if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return contains(MATLAB_Keywords, str);
    if (lang == Lang::PY || lang == Lang::IPYNB) return contains(PY_Keywords, str);
    else return false; // though impossible here
}

inline std::string Export::_asVarName(const std::string& str) const {
    if (_isKeyword(str)) return str + "_";
    else return str;
}

inline std::ofstream& Export::_wComment() {
    _f() << _langCommentSymbol() << ' ';
    return _f();
}

#endif
