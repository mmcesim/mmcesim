/**
 * @file type_track.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Type_Track Class
 * @version 0.1.0
 * @date 2022-09-24
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
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
    _blocks.push(_types.size());
}

void Type_Track::operator--(int) {
    if (_blocks.empty()) {
        throw std::out_of_range("-- operator in Type_Track out of range.");
    } else {
        // end of a block, erase all elements within the block
        _types.erase(_types.begin() + _blocks.top());
        _blocks.pop();
    }
}

void Type_Track::push(const std::string& var, const Type& type) {
    _types.push_back( { var, type } );
}

void Type_Track::push(const std::string& var, const std::string& type) {
    _types.push_back( { var, Type(type) } );
}
