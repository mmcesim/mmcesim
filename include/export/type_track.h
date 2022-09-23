/**
 * @file type_track.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Track data type of variables
 * @version 0.1.0
 * @date 2022-09-24
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _EXPORT_TYPE_TRACK_
#define _EXPORT_TYPE_TRACK_

#include <vector>
#include <stack>
#include <utility>
#include "export/type.h"

using Type_Pair = std::pair<std::string, Type>;

class Type_Track {
public:
    Type operator[](const std::string& var);

    void operator++(int);

    void operator--(int);

    void push(const std::string& var, const Type& type);

    void push(const std::string& var, const std::string& type);

private:
    std::vector<Type_Pair> _types;
    std::stack<size_t> _blocks; // the index of the starting block
};

#endif
