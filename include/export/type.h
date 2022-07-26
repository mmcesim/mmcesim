/**
 * @file type.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Type Specification in Alg
 * @version 0.1.0
 * @date 2022-07-26
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include <string>
#include "utils.h"

class Type {
    /**
     * @brief Data type specification
     * 
     * The type is specified with one character (e.g. 'c', 'f', etc.).
     * @note There are characters reserved for simple representation:
     *       - v = c1 : (column) complex vector;
     *       - r = c2 : row complex vector (viewed as matrix);
     *       - m = c2 : complex matrix;
     *       - d = f0 : float scalar (d);
     */
    enum class Data {
        COMPLEX,   /**< c */
        FLOAT,     /**< f */
        INTEGER,   /**< i */
        UNSIGNED,  /**< u */
        BOOL,      /**< b */
        STRING,    /**< s */
        CHAR,      /**< h */
        OTHER,     /**< o (reserved for future use) */
        UNKNOWN    /**< (unknown) */
    };
    using Dim = int8_t;

public:
    Type() = default;

    Type(const std::string& str);

    Data data() const noexcept;

    Dim dim() const noexcept;

    std::string string() const noexcept;

private:
    Data _data;
    Dim _dim;
};

inline Type::Data Type::data() const noexcept {
    return _data;
}

inline Type::Dim Type::dim() const noexcept {
    return _dim;
}

#endif
