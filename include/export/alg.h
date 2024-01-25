/**
 * @file alg.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Algorithm Parser (.alg Language)
 * @version 0.3.0
 * @date 2024-01-21
 *
 * @copyright Copyright (c) 2022-2024 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_ALG_H_
#define _EXPORT_ALG_H_

#include "_boost_config.h"
#include "export/alg_line.h"
#include "export/alg_opt.h"
#include "export/calc.h"
#include "export/macro.h"
#include "export/type.h"
#include "utils.h"
#include <boost/algorithm/string.hpp>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <stack>

class Alg {
  public:
    struct Error {
        std::string msg;
        std::string raw_str;
        size_t line;
    };
    using Warning     = Error;
    using Errors      = std::vector<Error>;
    using Warnings    = std::vector<Warning>;
    using Alg_Lines   = std::vector<Alg_Line>;
    using Raw_Strings = std::vector<std::string>;
    using Line_Nos    = std::vector<Raw_Strings::size_type>;
    using Keys        = std::vector<std::string>;

    struct Var {
        std::string name;
        /**
         * Dimension of the variable.
         * -1 for unknown.
         * 0 for scalar;
         * 1 for vector;
         * 2 for matrix;
         * 3 for tensor (cube).
         */
        int8_t dim = 0;
    };
    using Vars = std::vector<Var>;

  public:
    Alg() = default;

    Alg(const std::string& str, const Macro& macro = macro_none, int job_cnt = -1, int alg_cnt = -1,
        bool fail_fast = false, bool add_comment = false, bool add_semicolon = true, ALG_Opt opt = ALG_Opt::NONE);

    const Alg_Lines& lines() const;

    const Alg_Line& line(Alg_Lines::size_type index) const;

    const Errors& errors() const;

    const Error& error(Errors::size_type index) const;

    const Warnings& warnings() const;

    const Warning& warning(Errors::size_type index) const;

    bool hasError() const noexcept;

    bool write(std::ofstream& f, const std::string& lang);

    std::string inlineCalc(const std::string& s, const std::string& lang);

    void setIndent(bool use_space = true, unsigned indent_size = 4);

  private:
    std::ofstream& _wComment(std::ofstream& f, const std::string& lang, const std::string& before = "");

    std::string _indent(size_t indent_size) const noexcept;

    bool _applyKey(Alg_Line& line, const Keys& keys) const;

    void _recoverPrint() const;

    Alg_Lines _lines;
    Errors _errors;
    Warnings _warnings;
    Raw_Strings _raw_strings;
    Line_Nos _line_nos;
    Vars _vars;
    bool _failed          = false;
    bool _use_space       = false;
    bool _add_semicolon   = true;
    bool _add_comment     = true;
    unsigned _indent_size = 4;
    std::stack<std::string> _contents_at_end;
    Macro _macro;
    int _alg_cnt     = 0;
    int _job_cnt     = 0;
    int _recover_cnt = 0;
    std::vector<std::string> _recover_cnt_var;
    int _branch_line = Alg::max_length;

    const static int max_length = 100000;
};

inline const Alg::Alg_Lines& Alg::lines() const { return _lines; }

inline const Alg_Line& Alg::line(Alg_Lines::size_type index) const {
    assert((index < _lines.size() && "Check Alg::line index range."));
    return _lines[index];
}

inline const Alg::Errors& Alg::errors() const { return _errors; }

inline const Alg::Error& Alg::error(Errors::size_type index) const {
    assert((index < _errors.size() && "Check Alg::error index range."));
    return _errors[index];
}

inline const Alg::Warnings& Alg::warnings() const { return _warnings; }

inline const Alg::Warning& Alg::warning(Warnings::size_type index) const {
    assert((index < _warnings.size() && "Check Alg::warning index range."));
    return _warnings[index];
}

inline bool Alg::hasError() const noexcept { return !_errors.empty(); }

inline void Alg::setIndent(bool use_space, unsigned indent_size) {
    _use_space   = use_space;
    _indent_size = indent_size;
}

inline std::string Alg::_indent(size_t indent_size) const noexcept {
    if (_use_space) return std::string(indent_size, ' ');
    else return std::string(indent_size, '\t');
}

inline bool Alg::_applyKey(Alg_Line& line, const Keys& keys) const {
    if (line.params().size() > keys.size()) {
        return false;
    } else {
        for (decltype(keys.size()) i = 0; i != line.params().size(); ++i) {
            // Set the key until the key is specified by user.
            if (!line.setKey(i, keys[i])) break;
        }
        return true;
    }
}

#endif
