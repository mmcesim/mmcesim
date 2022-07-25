/**
 * @file alg.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Algorithm Parser (.alg Language)
 * @version 0.1.0
 * @date 2022-07-25
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _ALG_H_
#define _ALG_H_

#include "export/alg_line.h"

class Alg {
public:
    struct Error {
        std::string msg;
        std::string raw_str;
        size_t line;
    };
    using Errors = std::vector<Error>;
    using Alg_Lines = std::vector<Alg_Line>;

public:
    Alg() = default;

    Alg(const std::string& str, bool fail_fast = false);

    const Alg_Lines& lines() const;

    const Alg_Line& line(Alg_Lines::size_type index) const;

    const Errors& errors() const;

    const Error& error(Errors::size_type index) const;

    bool hasError() const noexcept;

private:
    Alg_Lines _lines;
    Errors _errors;
};

inline const Alg::Alg_Lines& Alg::lines() const {
    return _lines;
}

inline const Alg_Line& Alg::line(Alg_Lines::size_type index) const {
    assert((index < _lines.size() && "Check Alg::line index range."));
    return _lines[index];
}

inline const Alg::Errors& Alg::errors() const {
    return _errors;
}

inline const Alg::Error& Alg::error(Errors::size_type index) const {
    assert((index < _errors.size() && "Check Alg::error index range."));
    return _errors[index];
}

inline bool Alg::hasError() const noexcept {
    return !_errors.empty();
}

#endif
