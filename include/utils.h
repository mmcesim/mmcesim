/**
 * @file utils.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Utilities
 * @version 0.1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <algorithm>
#include <cctype>
#include <string>
#include <random>
#ifndef __linux__
#include <boost/dll/runtime_symbol_info.hpp>
#else
#include <libgen.h>       // dirname
#include <unistd.h>       // readlink
#include <linux/limits.h> // PATH_MAX
#endif

// https://stackoverflow.com/a/217605/15080514

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

// If the string is quoted with a character
static inline bool isQuoted(const std::string& s, char c = '"') {
    if (s.length() < 2) return false;
    if (s[0] == c && *(s.end() - 1) == c) return true;
    else return false;
}

// If the string is quoted, remove the quote.
static std::string removeQuote(const std::string& s) {
    if (isQuoted(s, '"') || isQuoted(s, '\'')) {
        // If the above statement is true,
        // 's' is surely longer than 2.
        return s.substr(1, s.length() - 2);
    } else {
        return s;
    }
}

// check if STL container contains a value
template<typename T>
static inline bool contains(const T& container, const typename T::value_type value) {
    for (const auto elem : container) {
        if (elem == value) return true;
    }
    return false;
}

// get the application directory
static inline std::string appDir() {
#ifndef __linux__
    // application directory
    return boost::dll::program_location().remove_filename().string();
#else
    char result[PATH_MAX] = {};
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    const char* path = "";
    if (count != -1) path = dirname(result);
    return path;
#endif
}

// random string
// Reference: https://stackoverflow.com/a/24586587/15080514
static std::string randomString(std::string::size_type length) {
    static auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);
    std::string s;
    s.reserve(length);
    while (length--) {
        s += chrs[pick(rg)];
    }
    return s;
}

#endif
