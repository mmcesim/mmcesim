/**
 * @file term.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Terminal Color and Style Control
 * @version 0.1.1
 * @date 2023-01-12
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _TERM_H_
#define _TERM_H_

#include <iostream>
#include <string>

/**
 * @brief Terminal option.
 * 
 * This will be directly used in ostream.
 */
class TermOpt {
  public:
    /**
     * @brief Construct a new TermOpt object.
     * 
     * @param val The ANSI color code in string.
     */
    TermOpt(const std::string& val) : v(val) {}

    /**
     * @brief Construct a new TermOpt object.
     * 
     * @param val The ANSI color code in const char*.
     */
    TermOpt(const char* val) : v(val) {}

    // ANSI color code
    std::string v;

    // valid status (whether a colorful terminal is used)
    bool vld = true;
};

/**
 * @brief Use the TermOpt in ostream.
 * 
 * @param out The ostream.
 * @param opt The TermOpt object with configuration.
 * @return (std::ostream&) The ostream.
 */
static inline std::ostream& operator<<(std::ostream& out, const TermOpt& opt) {
    if (opt.vld) out << "\033[" + opt.v + "m";
    return out;
}

/**
 * @brief Combine two TermOpt configurations.
 * 
 * @param o1 The first TermOpt configuration.
 * @param o2 The second TermOpt configuration.
 * @return (TermOpt) The added result.
 */
static inline TermOpt operator+(const TermOpt& o1, const TermOpt& o2) {
    return o1.v + ";" + o2.v;
}

/**
 * @brief Combine two TermOpt configurations.
 * 
 * This is the same as operator+.
 * @note This should be used with caution since the priority is lower than the << operator.
 * @param o1 The first TermOpt configuration.
 * @param o2 The second TermOpt configuration.
 * @return (TermOpt) The added result.
 */
static inline TermOpt operator|(const TermOpt& o1, const TermOpt& o2) {
    return o1 + o2;
}

/**
 * @brief Control terminal colors and styles.
 *
 */
class Term {
  public:
    static inline TermOpt BLACK = "30";

    /**
     * @brief Disable terminal color and style.
     * 
     */
    static void disable();

    /**
     * @brief Enable terminal color and style.
     * 
     * The color and style is on by default.
     */
    static void enable();
};

#endif
