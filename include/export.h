/**
 * @file export.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Export mmCEsim Configuration to Other Programming Languages
 * @version 0.2.2
 * @date 2023-04-06
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_H_
#define _EXPORT_H_

#include "cli_options.h"
#include "error_code.h"
#include "export/alg.h"
#include "export/channel_graph.h"
#include "export/keywords.h"
#include "export/shared_info.h"
#include "export/value_vec.h"
#include "read.h"
#include "term.h"
#include "utils.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <ctime>
#include <exception>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <tuple>
#include <type_traits>
#include <vector>

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

    enum DType : unsigned {
        INT    = 1,    ///< int
        DOUBLE = 2,    ///< double
        STRING = 4,    ///< string
        BOOL   = 8,    ///< bool
        CHAR   = 16,   ///< map
        SEQ    = 32,   ///< sequence
        MAP    = 64,   ///< map
        NUL    = 1024, ///< null
        UNDEF  = 2048  ///< undefined
    };

    /**
     * @brief Role of each node.
     *
     * RIS is the default role.
     */
    enum class NodeRole {
        Tx,  ///< transmitter
        Rx,  ///< receiver
        RIS, ///< RIS/IRS
    };

    Export(CLI_Options& opt, Shared_Info* const info = nullptr);

    Export(CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors, Shared_Info* const info = nullptr);

    ~Export();

    YAML_Errors exportCode();

    static YAML_Errors exportCode(CLI_Options& opt, Shared_Info* const info = nullptr);

    static YAML_Errors exportCode(CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors,
                                  Shared_Info* const info = nullptr);

  private:
    std::ofstream& _f();

    std::string _langStr() const;

    std::string _langName() const;

    std::string _langExtension() const;

    std::string _langHeaderExtension() const;

    std::string _langMmcesimExtension() const;

    std::string _langCommentSymbol() const;

    void _info(const std::string& str) const;

    // error message can be specified later
    bool _preCheck(const YAML::Node& node, unsigned allowed_type, bool mattered = true);

    template <typename T>
    T _as(const YAML::Node& n, bool mattered = true);

    std::string _asStr(const YAML::Node& n, bool mattered = true);

    void _setLatestError(const std::string& str);

    void _setLang();

    bool _isKeyword(const std::string& str) const;

    std::tuple<bool, std::string, std::string> _setChannelGains(const YAML::Node& n);

    std::string _asVarName(const std::string& str) const;

    std::ofstream& _wComment();

    void _topComment();

    void _beginning();

    void _generateChannels();

    void _algorithms();

    void _sounding();

    void _estimation(const Macro& macro, int job_cnt = -1);

    void _reporting();

    void _ending();

    bool _loadALG();

    bool _setCascadedChannel();

    bool _setMaxTestNum();

    /**
     * @brief Set the variable names defined by users.
     *
     * @retval true Success.
     * @retval false Failure.
     */
    bool _setVarNames();

    /**
     * @brief Generate RIS reflection matrices.
     *
     * @details To generate the RIS reflection matrices (i.e. the passive beamforming at the RIS),
     *          four parameters should be known:
     *            -# Pilot;
     *            -# Beam Number at the Tx;
     *            -# RIS size;
     *            -# The variable names.
     *
     *          The first two are necessary because we need to calculate the number of RIS reflection patterns;
     *          The variable names are also needed since we need to generate those definitions.\n
     *          The pilot can be directly written as 'pilot';
     *          The size of RIS can be obtained in the list;
     *          The variable names are listed in _beamforming_RIS.
     * @param Nt_B The beam number at the transmitter (Tx).
     */
    void _generateBF(unsigned Nt_B);

    unsigned _getTestNum(const YAML::Node& n);

    /**
     * @brief Get the size of the node (Tx/Rx/RIS).
     *
     * @param n The node.
     * @return (std::tuple<unsigned, unsigned, unsigned, unsigned, unsigned, unsigned>) The size.
     *         The five elements are: Mx, My, GMx, GMy, BMx, BMy.
     */
    std::tuple<unsigned, unsigned, unsigned, unsigned, unsigned, unsigned> _getSize(const YAML::Node& n);

    void _checkALGdependency(std::vector<std::string>& algs, bool logged = true);

    CLI_Options& _opt;
    YAML::Node _config;
    YAML_Errors _errors;
    bool _already_error_before_export = false;
    std::ofstream* _f_ptr             = nullptr;
    Shared_Info* const _s_info;
    std::vector<int> _transmitters;
    std::vector<int> _receivers;
    unsigned _max_test_num = 0;
    std::string _cascaded_channel;
    std::string _received_signal;
    std::string _noise;
    std::string _beamforming_W, _beamforming_F;
    std::vector<std::string> _beamforming_RIS;
    std::map<std::string, std::string> _beamforming;
    Channel_Graph _channel_graph;

    const int _MAX_TX = 1;
    const int _MAX_RX = 1;
};

template <typename T>
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
    if (l) return n.as<T>();
    else if (mattered) throw("Invalid!");
    else return T();
}

inline std::ofstream& Export::_f() { return *_f_ptr; }

inline std::string Export::_langStr() const {
    if (lang == Lang::CPP) return "cpp";
    else if (lang == Lang::MATLAB) return "matlab";
    else if (lang == Lang::OCTAVE) return "octave";
    else if (lang == Lang::PY) return "py";
    else if (lang == Lang::IPYNB) return "ipynb";
    else return "Impossible branch in \"Export::_langStr()\"!";
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

inline void Export::_info(const std::string& str) const { std::cout << "[mmCEsim] export $ " << str << std::endl; }

inline std::string Export::_asStr(const YAML::Node& n, bool mattered) { return _as<std::string>(n, mattered); }

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

// TODO: use this function in the future
inline std::string Export::_asVarName(const std::string& str) const {
    if (_isKeyword(str)) return str + "_";
    else return str;
}

inline std::ofstream& Export::_wComment() {
    _f() << _langCommentSymbol() << ' ';
    return _f();
}

inline unsigned Export::_getTestNum(const YAML::Node& n) {
    if (auto&& num = n["test_num"]; _preCheck(num, DType::INT, false)) {
        return num.as<unsigned>();
    } else return 500; // default value
}

#endif
