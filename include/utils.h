/**
 * @file utils.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Utilities
 * @version 0.3.0
 * @date 2023-07-29
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <algorithm>
#include <cctype>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#ifndef __linux__
#    include "_boost_config.h"
#    include <boost/dll/runtime_symbol_info.hpp>
#else
#    include <libgen.h>       // dirname
#    include <linux/limits.h> // PATH_MAX
#    include <unistd.h>       // readlink
#endif
#ifndef _WIN32
#    include <pwd.h>
#    include <sys/types.h>
#    include <unistd.h>
#endif

using std::string_literals::operator""s;

// https://stackoverflow.com/a/217605/15080514

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
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

template <typename T>
T strAs(const std::string& s) {
    std::stringstream ss(s);
    T t;
    ss >> t;
    return t;
}

/**
 * @brief Check if the string is an unsigned integer.
 *
 * @param s the string
 * @note Ref: https://stackoverflow.com/a/4654718/15080514
 */
static inline bool isUInt(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
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

// remove all white space in a string
static inline std::string& removeSpaceInPlace(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char x) { return std::isspace(x); }), str.end());
    return str;
}

// remove all white space in a string and make a copy
static inline std::string removeSpaceCopy(const std::string& str) {
    std::string new_str = str;
    new_str.erase(std::remove_if(new_str.begin(), new_str.end(), [](unsigned char x) { return std::isspace(x); }),
                  new_str.end());
    return new_str;
}

// get a vector of string as one string
static std::string stringVecAsString(const std::vector<std::string>& l, std::string div = " ") {
    if (l.empty()) return "";
    std::string str;
    for (auto iter = l.cbegin(); iter != l.cend() - 1; ++iter) { str += *iter + div; }
    return str + *(l.cend() - 1);
}

// check if STL container contains a value
template <typename T>
static inline bool contains(const T& container, const typename T::value_type value) {
    for (auto&& elem : container) {
        if (elem == value) return true;
    }
    return false;
}

#ifndef _WIN32
static inline std::string homeDir() {
    const char* homedir = std::getenv("HOME");
    return (homedir && homedir[0] != '~') ? homedir : getpwuid(getuid())->pw_dir;
}
#endif

// get the application directory
static inline std::string appDir() {
#ifndef __linux__
    // application directory
    return boost::dll::program_location().remove_filename().string();
#else
    char result[PATH_MAX] = {};
    ssize_t count         = readlink("/proc/self/exe", result, PATH_MAX);
    const char* path      = "";
    if (count != -1) path = dirname(result);
    return path;
#endif
}

// get the data directory (i.e., log file directory)
static inline std::string dataDir() {
#ifdef _WIN32
    return appDir();
#else
    // Reference: https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
    const char* XDG_DATA_HOME = std::getenv("XDG_DATA_HOME");
    return XDG_DATA_HOME ? (XDG_DATA_HOME[0] == '~' ? homeDir() + std::string(XDG_DATA_HOME).substr(1)
                                                    : std::string(XDG_DATA_HOME))
                         : homeDir() + "/.local/share";
#endif
}

// random string
// Reference: https://stackoverflow.com/a/24586587/15080514
static std::string randomString(std::string::size_type length) {
    static auto& chrs = "0123456789"
                        "abcdefghijklmnopqrstuvwxyz"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    thread_local static std::mt19937 rg{ std::random_device{}() };
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);
    std::string s;
    s.reserve(length);
    while (length--) { s += chrs[pick(rg)]; }
    return s;
}

/**
 * @brief Utilities specified for mmCEsim
 *
 * @details This is used because this is in violation to the normal std namespace.
 */
namespace mmce {
/**
 * @brief Change a number to string.
 *
 * @details This is useful because we do not want trailing 0 for a float number conversion.
 * @tparam T The number data type.
 * @param x The number.
 * @return (std::string) The converted string.
 */
template <typename T>
static inline std::string to_string(const T& x) {
    std::ostringstream str;
    str << x;
    return str.str();
}
} // namespace mmce

#endif
