/**
 * @file functions.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Function Lists
 * @version 0.2.1
 * @date 2023-04-06
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
    "BRANCH"s, "BREAK"s,  "CALC"s,  "CALL"s, "COMMENT"s, "CPP"s,   "ESTIMATE"s, "INIT"s,
    "LOG"s,    "MATLAB"s, "MERGE"s, "NEW"s,  "OCTAVE"s,  "PRINT"s, "PYTHON"s,   "RECOVER"s,
};

static std::array functions_needs_end = {
    "ELSE"s, "ELIF"s, "FOR"s, "FOREVER"s, "FUNCTION"s, "IF"s, "LOOP"s, "WHILE"s,
};

static std::array functions_is_end = {
    "END"s,
    "ELSE"s,
    "ELIF"s,
};

static inline bool isFuncNoEnd(const std::string& str) noexcept { return contains(functions_no_end, str); }

static inline bool isFuncNeedsEnd(const std::string& str) noexcept { return contains(functions_needs_end, str); }

static inline bool isFuncIsEnd(const std::string& str) noexcept { return contains(functions_is_end, str); }

static inline bool isFunc(const std::string& str) noexcept {
    return (isFuncNoEnd(str) || isFuncNeedsEnd(str) || isFuncIsEnd(str));
}

#endif
