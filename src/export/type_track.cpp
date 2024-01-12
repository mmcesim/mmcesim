/**
 * @file type_track.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Type_Track Class
 * @version 0.2.2
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "export/type_track.h"

Type Type_Track::operator[](const std::string& var) {
    for (int i = _types.size() - 1; i >= 0; --i) {
        auto&& pair = _types[i];
        if (var == pair.first) return pair.second;
    }
    return Type(); // default as "UNKOWN" (which is the default constructor of Type)
}

void Type_Track::operator++(int) {
    _scopes.push(_types.size());
#ifdef _TYPE_TRACK_PRINT_INFO
    _log.info() << "{Type Track} Scope starts with size " << size() << std::endl;
#endif
}

void Type_Track::operator--(int) {
#ifdef _TYPE_TRACK_PRINT_INFO
    _log.info() << "{Type Track} Scope ends with size " << size() << std::endl;
#endif
    if (_scopes.empty()) {
        _log.err() << "(Internal Error) Type Track: -- operator in Type_Track out of range." << std::endl;
        throw std::out_of_range("-- operator in Type_Track out of range.");
    } else {
        // end of a scope, erase all elements within the scope
        _types.erase(_types.begin() + _scopes.top(), _types.end());
        _scopes.pop();
    }
}

void Type_Track::push(const std::string& var, const Type& type) {
#ifdef _TYPE_TRACK_PRINT_INFO
    _log.info() << "{Type Track} Push '" << var << "' of type '" << type.string() << "'" << std::endl;
#endif
    _types.push_back({ var, type });
}

void Type_Track::push(const std::string& var, const std::string& type) {
#ifdef _TYPE_TRACK_PRINT_INFO
    _log.info() << "{Type Track} Push '" << var << "' of type " << type << std::endl;
#endif
    _types.push_back({ var, Type(type) });
}
