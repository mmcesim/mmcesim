/**
 * @file type_track.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Track data type of variables
 * @version 0.2.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_TYPE_TRACK_
#define _EXPORT_TYPE_TRACK_

#include "export/type.h"
#include "log_global.h"
#include <iostream>
#include <stack>
#include <stdexcept>
#include <utility>
#include <vector>

#define _TYPE_TRACK_PRINT_INFO

using Type_Pair = std::pair<std::string, Type>;

class Type_Track {
  public:
    Type operator[](const std::string& var);

    void operator++(int);

    void operator--(int);

    void push(const std::string& var, const Type& type);

    void push(const std::string& var, const std::string& type);

    size_t size() const noexcept;

    bool empty() const noexcept;

  private:
    std::vector<Type_Pair> _types;
    std::stack<size_t> _scopes; // the index of the starting scope
};

inline size_t Type_Track::size() const noexcept { return _types.size(); }

inline bool Type_Track::empty() const noexcept { return _types.empty(); }

#endif
