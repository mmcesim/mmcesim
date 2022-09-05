/**
 * @file type.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Alg Class
 * @version 0.1.0
 * @date 2022-07-27
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include "export/type.h"

Type::Type(const std::string& str) {
    switch (auto l = str.length()) {
    case 0:
        _data = Data::UNKNOWN;
        break;
    case 1:
        std::tie(_data, _dim) = _getData(str[0]);
        // If not specified, the dimension is 0 (as a scalar).
        if (_dim == -1) _dim = 0;
        break;
    default:
        std::tie(_data, _dim) = _getData(str[0]);
        // If not specified, the simplified representation is not used in the first character.
        bool has_number;
        if (_dim == -1) {
            switch (str[1]) {
            case '0': _dim = 0; break;
            case '1': _dim = 1; break;
            case '2': _dim = 2; break;
            case '3': _dim = 3; break;
            default:  _dim = -1;
            }
            has_number = true;
        } else {
            char dim_unused;
            if (auto c = str[1]; isdigit(c)) {
                Dim dim_unused = c - '0';
                // If the user specified dimension is the same as the first simplification representation,
                // we now pretend nothing has happened. (For example, 'm2' is just 'm' or 'c2'.)
                // Else there is a conflict, so we set the dimension to unkown (-1).
                if (dim_unused != _dim) {
                    _dim = -1; // raise conflict error
                }
            }
            has_number = false;
        }
        if (has_number) {
            if (l >= 3) {
                _suffix = _getSuffix(str[2]);
            }
        } else {
            _suffix = _getSuffix(str[1]);
        }
        break;
    }
}

std::string Type::string() const noexcept {
    if (isUnknown()) return "";
    std::string type = _suffix == Suffix::CONST_ ? "const " : "";
    if (_data == Data::COMPLEX) {
        if (_dim == 0) type += "std::complex<double>";
        else if (_dim == 1) type += "cx_vec";
        else if (_dim == 2) type += "cx_mat";
        else if (_dim == 3) type += "cx_cube";
        else { /* impossible */ };
    } else if (_data == Data::FLOAT) {
        if (_dim == 0) type += "double";
        else if (_dim == 1) type += "vec";
        else if (_dim == 2) type += "mat";
        else if (_dim == 3) type += "cube";
        else { /* impossible */ };
    } else if (_data == Data::INTEGER) {
        return _getString("int");
    } else if (_data == Data::UNSIGNED) {
        if (_dim == 0) type += "uword";
        else if (_dim == 1) type += "uvec";
        else if (_dim == 2) type += "umat";
        else if (_dim == 3) type += "ucube";
    } else if (_data == Data::BOOL) {
        return _getString("bool");
    } else if (_data == Data::STRING) {
        return _getString("std::string");
    } else if (_data == Data::CHAR) {
        return _getString("char");
    } else {
        return "";
    }
    return type;
}

std::tuple<Type::Data, Type::Dim> Type::_getData(char c) const noexcept {
    switch (tolower(c)) {
    case 'c': return { Data::COMPLEX,  -1 };
    case 'f': return { Data::FLOAT,    -1 };
    case 'i': return { Data::INTEGER,  -1 };
    case 'u': return { Data::UNSIGNED, -1 };
    case 'b': return { Data::BOOL,     -1 };
    case 's': return { Data::STRING,   -1 };
    case 'h': return { Data::CHAR,     -1 };
/*  case 'o': return { Data::OTHER,    -1 }; */
    case 'v': return { Data::COMPLEX,   1 };
    case 'r': return { Data::COMPLEX,   2 };
    case 'm': return { Data::COMPLEX,   2 };
    case 't': return { Data::COMPLEX,   3 };
    case 'd': return { Data::FLOAT,     0 };
    default:  return { Data::UNKNOWN,  -1 };
    }
}

Type::Suffix Type::_getSuffix(char c) const noexcept {
    switch (tolower(c)) {
    case 'c': return Suffix::CONST_;
/*  case 'v': return Suffix::VOLATILE_; */
    case 0:   return Suffix::NONE;
    default:  return Suffix::UNKNOWN;
    }
}

std::string Type::_getString(const std::string& elem_type) const noexcept {
    if (_dim == 0) return elem_type;
    if (_dim == 1) return std::string("Col<") + elem_type + ">";
    if (_dim == 2) return std::string("Mat<") + elem_type + ">";
    if (_dim == 3) return std::string("Cube<") + elem_type + ">";
    else return ""; // though it should not come here
}
