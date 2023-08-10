/**
 * @file alg_line.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Parse Line of Alg
 * @version 0.2.2
 * @date 2023-04-06
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_ALG_LINE_H_
#define _EXPORT_ALG_LINE_H_

#include "export/alg_opt.h"
#include "export/functions.h"
#include "log_global.h"
#include "utils.h"
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Process line of Alg into standard formats.
 *
 * The main process is done during constructing.
 */
class Alg_Line {
  public:
    /**
     * @brief Return type.
     *
     * Part that is left of '='.
     */
    struct Return_Type {
        std::string name; /**< name of the return variable */
        std::string type; /**< data type of the return type */
    };

    /**
     * @brief Parameter type.
     *
     * Part that is right of '=' except for the function name.
     */
    struct Param_Type {
        std::string key;   /**< the key of the parameter */
        std::string value; /**< the value of the parameter */
        std::string type;  /**< the data type of the parameter */
    };

  public:
    // Default constructor.
    Alg_Line() = default;

    /**
     * @brief Construct a new Alg_Line object.
     *        Process string at the same time.
     *
     * @param str The raw string from Alg that is to be processed.
     * @param opt ALG parsing options.
     */
    Alg_Line(const std::string& str, ALG_Opt opt = ALG_Opt::NONE);

    /**
     * @brief Get the function name.
     *
     * @return (const std::string&) The function name.
     */
    const std::string& func() const noexcept;

    /**
     * @brief Return variables.
     *
     * @return (const std::vector<Return_Type>&) The vector of return variables.
     */
    const std::vector<Return_Type>& returns() const noexcept;

    /**
     * @brief Return variable of an index.
     *
     * @param index The index of the return variable.
     * @return (const Return_Type&) The variable.
     */
    const Return_Type& returns(std::vector<Return_Type>::size_type index) const;

    /**
     * @brief Parameter variables.
     *
     * @return (const std::vector<Param_Type>&) The vector of parameter variables.
     */
    const std::vector<Param_Type>& params() const noexcept;

    /**
     * @brief Parameter variable of an index.
     *
     * @param index The index of the parameter index.
     * @return (const Param_Type&) The variable.
     */
    const Param_Type& params(std::vector<Param_Type>::size_type index) const;

    /**
     * @brief Parameter variable with a key.
     *
     * @param key The key string.
     * @return (const Param_Type&) The parameter variable.
     */
    const Param_Type& params(const std::string& key) const;

    /**
     * @brief Get the parameter value with the key.
     *
     * @param key The key string.
     * @return (const std::string&) The parameter variable value.
     */
    const std::string& operator[](const std::string key) const;

    /**
     * @brief Return the raw string of the line.
     *
     * @return (const std::string&) The raw string.
     */
    const std::string& rawStr() const;

    /**
     * @brief Check whether parameters contain the key.
     *
     * Normally, the function 'setKey' needs to be called first
     * so that parameters specified by locations can be found as well.
     * @param key The key string to check.
     * @retval true Parameter variables contain this key.
     * @retval false Parameter variables do not contain this key.
     */
    bool hasKey(const std::string& key) const noexcept;

    /**
     * @brief Check if parameters have unknown keys.
     *
     * @param keys All valid keys.
     * @retval true All keys of parameters are valid.
     * @retval false There exist unknown keys.
     */
    bool hasUnknownKey(const std::vector<std::string>& keys) const noexcept;

    /**
     * @brief Check if parameters have repeated keys.
     *
     * @retval true The parameters have repeated keys.
     * @retval false The parameters do not have repeated keys.
     */
    bool hasRepeatedKey() const noexcept;

    /**
     * @brief Check is the key is valid. (not violating the needed sequence.)
     *
     * If the key does not exist, it is not valid;
     * If the key exists, the keys need to exist.
     * For example, for size specification,
     * if the third dimension ('dim3') exists,
     * the first two dimensions ('dim1' and 'dim2') also need to exist.
     * Similar to method 'hasKey',
     * normally this method should be called after 'setKey'.
     * @param key The key string to check.
     * @param keys The required keys if key exists.
     * @retval true The key is not valid.
     * @retval false The key is valid.
     */
    bool isValidKey(const std::string& key, const std::vector<std::string>& keys) const noexcept;

    /**
     * @brief Set parameter variable key.
     *
     * @param index The parameter index.
     * @param key The key string to assign.
     * @retval true The parameter does not have key previously.
     * @retval false The parameter has key before.
     */
    bool setKey(std::vector<Param_Type>::size_type index, const std::string& key);

    /**
     * @brief Test whether the function name needs an 'end'
     *
     * @retval true The function needs 'end'. ('IF', 'ELSE', 'LOOP', etc.)
     * @retval false The function does need 'end'.
     */
    bool needsEnd() const noexcept;

    /**
     * @brief Test whether the function name serves as an 'end'.
     *
     * @retval true The function serves as 'end'. ('END', 'ELSE', etc.)
     * @retval false The function does not serve as 'end'.
     */
    bool isEnd() const noexcept;

    /**
     * @brief Check whether the line is for function declaration.
     *
     * @retval true The ALG_Opt is set to ALG_Opt::FUNCTION_DECLARATION
     * @retval false The ALG_Opt is not ALG_Opt::FUNCTION_DECLARATION
     */
    bool isFunctionDeclaration() const noexcept;

    /**
     * @brief Print Alg_Line contents (including return values, function names and parameters).
     *
     * @details This is mostly used in internal debugging.
     * @param out The output stream.
     * @param prefix The print prefix.
     * @return (std::ostream&) The output stream.
     */
    std::ostream& print(std::ostream& out = std::cout, std::string prefix = "") const;

  private:
    /**
     * @brief Find the first occupance of substring in string that is not an escape or within quotes.
     *
     * @param s string
     * @param cs the substring to find
     * @return (std::string::size_type) The index of the found substring. Return s.size() if not found.
     */
    std::string::size_type _findChars(const std::string& s, std::string cs) const noexcept;

    /**
     * @brief Find the first occupance of character in string that is not an escape or within quotes.
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

    /**
     * @brief Process returns variables.
     *
     * @param v The unprocessed return variables token vector.
     */
    void _processReturns(const std::vector<std::string>& v);

    /**
     * @brief Process function name and parameter variables.
     *
     * @param v The unprocessed function name and parameter variables token variable.
     */
    void _processFuncParams(const std::vector<std::string>& v);

  private:
    std::string _func;                 /**< function name */
    std::vector<Return_Type> _returns; /**< return variables */
    std::vector<Param_Type> _params;   /**< parameter variables */
    std::string _raw_str;              /**< raw string (original line) */
    ALG_Opt _opt;                      /**< ALG options */
};

inline const std::string& Alg_Line::func() const noexcept { return _func; }

inline const std::vector<Alg_Line::Return_Type>& Alg_Line::returns() const noexcept { return _returns; }

inline const Alg_Line::Return_Type& Alg_Line::returns(std::vector<Return_Type>::size_type index) const {
    assert(index < _returns.size() && "Alg_Line check returns index size within bound.");
    return _returns[index];
}

inline const std::vector<Alg_Line::Param_Type>& Alg_Line::params() const noexcept { return _params; }

inline const Alg_Line::Param_Type& Alg_Line::params(std::vector<Param_Type>::size_type index) const {
    assert((index < _params.size() && "Alg_Line check params index size within bound."));
    return _params[index];
}

inline const Alg_Line::Param_Type& Alg_Line::params(const std::string& key) const {
    for (auto&& elem : _params) {
        if (elem.key == key) return elem;
    }
    throw("No such key!");
}

inline const std::string& Alg_Line::operator[](const std::string key) const { return params(key).value; }

inline const std::string& Alg_Line::rawStr() const { return _raw_str; }

inline bool Alg_Line::hasKey(const std::string& key) const noexcept {
    for (auto&& elem : _params) {
        if (elem.key == key) return true;
    }
    return false;
}

inline bool Alg_Line::hasUnknownKey(const std::vector<std::string>& keys) const noexcept {
    for (auto&& elem : _params) {
        if (!contains(keys, elem.key)) return false;
    }
    return true;
}

inline bool Alg_Line::hasRepeatedKey() const noexcept {
    std::vector<std::string> keys(_params.size());
    for (decltype(_params.size()) i = 0; i != _params.size(); ++i) { keys[i] = _params[i].key; }
    std::sort(keys.begin(), keys.end());
    return std::adjacent_find(keys.begin(), keys.end()) != keys.end();
}

inline bool Alg_Line::isValidKey(const std::string& key, const std::vector<std::string>& keys) const noexcept {
    if (!hasKey(key)) return false;
    std::vector<bool> exists(keys.size(), false);
    for (auto&& elem : _params) {
        auto&& the_key = elem.key;
        for (decltype(keys.size()) i = 0; i != keys.size(); ++i) {
            if (keys[i] == the_key) exists[i] = true;
        }
    }
    for (auto&& ck : exists) {
        if (!ck) return false;
    }
    return true;
}

inline bool Alg_Line::setKey(std::vector<Param_Type>::size_type index, const std::string& key) {
    assert(index < _params.size() && "Alg_Line check params index size within bound when setting key.");
    if (_params[index].key == "") {
        _params[index].key = key;
        return true;
    } else {
        return false;
    }
}

inline bool Alg_Line::needsEnd() const noexcept { return isFuncNeedsEnd(_func); }

inline bool Alg_Line::isEnd() const noexcept { return isFuncIsEnd(_func); }

inline bool Alg_Line::isFunctionDeclaration() const noexcept { return _opt == ALG_Opt::FUNCTION_DECLARATION; }

inline std::ostream& Alg_Line::print(std::ostream& out, std::string prefix) const {
    out << prefix << "$ " << _raw_str << '\n';
    if (_func.empty()) return out;
    out << prefix << "* FUNCTION: " << _func << '\n';
    if (_func != "COMMENT" && !_params.empty()) {
        out << prefix << "* PARAMS:\n";
        for (auto&& p : _params) out << prefix << "   > {" << p.key << "}={" << p.value << "}::{" << p.type << "}\n";
    }
    if (!isFuncNoEnd(_func)) {
        out << prefix << "* RETURN:\n";
        for (auto&& r : _returns) out << prefix << "   > {" << r.name << "}::{" << r.type << "}\n";
    }
    out << std::flush;
    return out;
}

inline std::string::size_type Alg_Line::_findChar(const std::string& s, char c) const noexcept {
    return _findChars(s, std::string(1, c));
}

/**
 * @brief Print Alg_Line contents (including return values, function names and parameters).
 *
 * @details This is mostly used in internal debugging. It internally called function 'print'.
 * @param out The output stream.
 * @param line The Alg_Line Object.
 * @return (std::ostream&) The output stream.
 */
static inline std::ostream& operator<<(std::ostream& out, const Alg_Line& line) { return line.print(out); }

#endif
