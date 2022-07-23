/**
 * @file alg_line.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Parse Line of Alg
 * @version 0.1.0
 * @date 2022-07-22
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _ALG_LINE_H_
#define _ALG_LINE_H_

#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include "utils.h"
#include "export/functions.h"

class Alg_Line {
public:
    struct Return_Type {
        std::string name;
        std::string type;
    };

    struct Param_Type {
        std::string key;
        std::string value;
        std::string type;
    };

public:
    Alg_Line() = default;

    Alg_Line(const std::string& str);

    const std::string& func() const noexcept;

    const std::vector<Return_Type>& returns() const noexcept;

    const Return_Type& returns(std::vector<Return_Type>::size_type index) const;

    const std::vector<Param_Type>& params() const noexcept;

    const Param_Type& params(std::vector<Param_Type>::size_type index) const;

    const Param_Type& params(const std::string& key) const;

    bool needsEnd() const noexcept;

private:
    /**
     * @brief Find the first occupance of character in string
     *        that is not an escape or within quotes.
     * 
     * @param s string
     * @param c the character to find
     * @return (std::string::size_type) The index of the found char. Return s.size() if not found.
     */
    std::string::size_type _findChar(const std::string& s, char c) const noexcept;

    /**
     * @brief Remove the comment from the string.
     * 
     * Comment starts with hash(#).
     * @param s The string to be modified.
     */
    void _removeComment(std::string& s) const noexcept;

    void _processReturns(const std::vector<std::string>& v);

    void _processFuncParams(const std::vector<std::string>& v);

private:
    std::string _func;
    std::vector<Return_Type> _returns;
    std::vector<Param_Type> _params;
};

inline const std::string& Alg_Line::func() const noexcept {
    return _func;
}

inline const std::vector<Alg_Line::Return_Type>& Alg_Line::returns() const noexcept {
    return _returns;
}

inline const Alg_Line::Return_Type& Alg_Line::returns(std::vector<Alg_Line::Return_Type>::size_type index) const {
    assert((index < _returns.size() && "Alg_Line check returns index size within bound."));
    return _returns[index];
}

inline const std::vector<Alg_Line::Param_Type>& Alg_Line::params() const noexcept {
    return _params;
}

inline const Alg_Line::Param_Type& Alg_Line::params(std::vector<Alg_Line::Param_Type>::size_type index) const {
    assert((index < _params.size() && "Alg_Line check params index size within bound."));
    return _params[index];
}

inline bool Alg_Line::needsEnd() const noexcept {
    return isFuncIsEnd(_func);
}

inline const Alg_Line::Param_Type& Alg_Line::params(const std::string& key) const {
    for (auto&& elem : _params) {
        if (elem.key == key) return elem;
    }
    throw("No such key!");
}

#endif
