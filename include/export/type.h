/**
 * @file type.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Type Specification in Alg
 * @version 0.1.0
 * @date 2022-07-27
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _TYPE_H_
#define _TYPE_H_

#include <string>
#include <tuple>

class Type {
    /**
     * @brief Data type specification.
     * 
     * The type is specified with one character (e.g. 'c', 'f', etc.).
     * @note There are characters reserved for simple representation:
     *       - v = c1 : (column) complex vector;
     *       - r = c2 : row complex vector (viewed as matrix);
     *       - m = c2 : complex matrix;
     *       - t = c3 : complex tensor (cube);
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

    /**
     * @brief Type prefix.
     * 
     * These are additional attributes of the type
     */
    enum class Suffix {
        NONE,      /**< (none) */
        CONST_,    /**< c */
        VOLATILE_, /**< v (not implemented yet) */
        UNKNOWN,   /** (unknown) */
    };

    using Dim = int8_t;

public:
    Type() = default;

    Type(const std::string& str);

    Data data() const noexcept;

    Dim dim() const noexcept;

    /**
     * @brief Get the C++ type name.
     * 
     * @return (std::string) C++ type name. Return empty string if it is unknown.
     */
    std::string string() const noexcept;

    bool isUnknown() const noexcept;

    static std::string string(const std::string&) noexcept;

private:
    std::tuple<Data, Dim> _getData(char c) const noexcept;

    Suffix _getSuffix(char c) const noexcept;

    std::string _getString(const std::string& elem_type) const noexcept;

private:
    Data _data = Data::UNKNOWN;
    Dim _dim = -1;
    Suffix _suffix = Suffix::NONE;
};

inline Type::Data Type::data() const noexcept {
    return _data;
}

inline Type::Dim Type::dim() const noexcept {
    return _dim;
}

inline bool Type::isUnknown() const noexcept {
    return (_data == Data::UNKNOWN || _dim == -1 || _suffix == Suffix::UNKNOWN);
}

inline static std::string string(const std::string& str) noexcept {
    Type t(str);
    return t.string();
}

#endif
