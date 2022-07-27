/**
 * @file alg_line.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Parse Line of Alg
 * @version 0.1.0
 * @date 2022-07-24
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
#include <algorithm>
#include <iomanip>
#include "utils.h"
#include "export/functions.h"

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
     */
    Alg_Line(const std::string& str);

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
     * @brief Check whether parameters contain the key.
     * 
     * Normally, the function 'setKey' needs to be called first
     * so that parameters specified by locations can be found as well.
     * @param key The key string to check.
     * @return true Parameter variables contain this key.
     * @return false Parameter variables do not contain this key.
     */
    bool hasKey(const std::string& key) const noexcept;

    /**
     * @brief Check if parameters have unknown keys.
     * 
     * @param keys All valid keys.
     * @return true All keys of parameters are valid.
     * @return false There exist unknown keys.
     */
    bool hasUnknownKey(const std::vector<std::string>& keys) const noexcept;

    /**
     * @brief Check if parameters have repeated keys.
     * 
     * @return true The parameters have repeated keys.
     * @return false The parameters do not have repeated keys.
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
     * @return true The key is not valid.
     * @return false The key is valid.
     */
    bool isValidKey(const std::string& key, const std::vector<std::string>& keys) const noexcept;

    /**
     * @brief Set parameter variable key.
     * 
     * @param index The parameter index.
     * @param key The key string to assign.
     * @return true The parameter does not have key previously.
     * @return false The parameter has key before.
     */
    bool setKey(std::vector<Param_Type>::size_type index, const std::string& key);

    /**
     * @brief Test whether the function name needs an 'end'
     * 
     * @return true The function needs 'end'. ('IF', 'ELSE', 'LOOP', etc.)
     * @return false The function does need 'end'.
     */
    bool needsEnd() const noexcept;

    /**
     * @brief Test whether the function name serves as an 'end'.
     * 
     * @return true The function serves as 'end'. ('END', 'ELSE', etc.)
     * @return false The function does not serve as 'end'.
     */
    bool isEnd() const noexcept;

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
    std::string _func; /**< function name */
    std::vector<Return_Type> _returns; /**< return variables */
    std::vector<Param_Type> _params; /**< parameter variables */
};

inline const std::string& Alg_Line::func() const noexcept {
    return _func;
}

inline const std::vector<Alg_Line::Return_Type>& Alg_Line::returns() const noexcept {
    return _returns;
}

inline const Alg_Line::Return_Type& Alg_Line::returns(std::vector<Return_Type>::size_type index) const {
    assert(index < _returns.size() && "Alg_Line check returns index size within bound.");
    return _returns[index];
}

inline const std::vector<Alg_Line::Param_Type>& Alg_Line::params() const noexcept {
    return _params;
}

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
    for (decltype(_params.size()) i = 0; i != _params.size(); ++i) {
        keys[i] = _params[i].key;
    }
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

inline bool Alg_Line::needsEnd() const noexcept {
    return isFuncNeedsEnd(_func);
}

inline bool Alg_Line::isEnd() const noexcept {
    return isFuncIsEnd(_func);
}

#endif
