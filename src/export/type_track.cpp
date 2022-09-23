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

}

void Type_Track::operator--(int) {

}

void Type_Track::push(const std::string& var, const Type& type) {

}

void Type_Track::push(const std::string& var, const std::string& type) {

}
