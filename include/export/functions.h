/**
 * @file functions.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Function Lists
 * @version 0.2.0
 * @date 2022-07-23
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_FUNCTIONS_H_
#define _EXPORT_FUNCTIONS_H_

#include "utils.h"
#include <array>
#include <string>

static std::array functions_no_end = {
    std::string("BRANCH"), std::string("BREAK"),    std::string("CALC"),  std::string("CALL"),   std::string("COMMENT"),
    std::string("CPP"),    std::string("ESTIMATE"), std::string("INIT"),  std::string("MATLAB"), std::string("MERGE"),
    std::string("NEW"),    std::string("OCTAVE"),   std::string("PRINT"), std::string("PYTHON"), std::string("RECOVER"),
};

static std::array functions_needs_end = {
    std::string("ELSE"),     std::string("ELIF"), std::string("FOR"),  std::string("FOREVER"),
    std::string("FUNCTION"), std::string("IF"),   std::string("LOOP"), std::string("WHILE"),
};

static std::array functions_is_end = {
    std::string("END"),
    std::string("ELSE"),
    std::string("ELIF"),
};

static inline bool isFuncNoEnd(const std::string& str) noexcept { return contains(functions_no_end, str); }

static inline bool isFuncNeedsEnd(const std::string& str) noexcept { return contains(functions_needs_end, str); }

static inline bool isFuncIsEnd(const std::string& str) noexcept { return contains(functions_is_end, str); }

static inline bool isFunc(const std::string& str) noexcept {
    return (isFuncNoEnd(str) || isFuncNeedsEnd(str) || isFuncIsEnd(str));
}

#endif
