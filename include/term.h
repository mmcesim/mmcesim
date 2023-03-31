/**
 * @file term.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Terminal Color and Style Control
 * @version 0.2.0
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _TERM_H_
#define _TERM_H_

#include <cstdlib>
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

    /// ANSI color code
    std::string v;

    /// valid status (whether a colorful terminal is used)
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
static inline TermOpt operator+(const TermOpt& o1, const TermOpt& o2) { return o1.v + ";" + o2.v; }

/**
 * @brief Combine two TermOpt configurations.
 *
 * This is the same as operator+.
 * @note This should be used with caution since the priority is lower than the << operator.
 * @param o1 The first TermOpt configuration.
 * @param o2 The second TermOpt configuration.
 * @return (TermOpt) The added result.
 */
static inline TermOpt operator|(const TermOpt& o1, const TermOpt& o2) { return o1 + o2; }

/**
 * @brief Control terminal colors and styles.
 *
 */
class Term {
  public:
    /* color and style definitions */
    static inline TermOpt BLACK      = "30";
    static inline TermOpt RED        = "31";
    static inline TermOpt GREEN      = "32";
    static inline TermOpt YELLOW     = "33";
    static inline TermOpt BLUE       = "34";
    static inline TermOpt MAGENTA    = "35";
    static inline TermOpt CYAN       = "36";
    static inline TermOpt WHITE      = "37";
    static inline TermOpt DEFAULT    = "39";
    static inline TermOpt BG_BLACK   = "40";
    static inline TermOpt BG_RED     = "41";
    static inline TermOpt BG_GREEN   = "42";
    static inline TermOpt BG_YELLOW  = "43";
    static inline TermOpt BG_BLUE    = "44";
    static inline TermOpt BG_MAGENTA = "45";
    static inline TermOpt BG_CYAN    = "46";
    static inline TermOpt BG_WHITE   = "47";
    static inline TermOpt BG_DEFAULT = "49";
    static inline TermOpt RESET      = "0";
    static inline TermOpt BF         = "1";  /**< bold/bright */
    static inline TermOpt DIM        = "2";  /**< dim (faint) */
    static inline TermOpt IT         = "3";  /**< italic */
    static inline TermOpt UL         = "4";  /**< underline */
    static inline TermOpt BLINK      = "5";  /**< slow blink (150 times/min) */
    static inline TermOpt INV        = "7";  /**< inverse FG and BG */
    static inline TermOpt OFF_BF     = "21"; /**< off bold/bright */
    static inline TermOpt OFF_DIM    = "22"; /**< dim (faint) */
    static inline TermOpt OFF_IT     = "23"; /**< italic */
    static inline TermOpt OFF_UL     = "24"; /**< off underline */
    static inline TermOpt OFF_BLINK  = "25"; /**< off slow blink (150 times/min) */
    static inline TermOpt OFF_INV    = "27"; /**< off inverse FG and BG */

    /* some additional compound definitions */
    static inline TermOpt ERR = RED | BF;    /**< error message */
    static inline TermOpt MSG = YELLOW;      /**< message */
    static inline TermOpt WAR = YELLOW | BF; /**< warning message */

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

    /**
     * @brief Print error message.
     *
     * @tparam T The message type.
     * @param msg The error message.
     */
    template <typename T>
    static void error(const T& msg);

    /**
     * @brief Print a message.
     *
     * @tparam T The message type.
     * @param msg The message.
     */
    template <typename T>
    static void message(const T& msg);

    /**
     * @brief Print a warning message.
     *
     * @tparam T The warning message type.
     * @param msg The warning message.
     */
    template <typename T>
    static void warning(const T& msg);

    /**
     * @brief Detect environment variable of NO_COLOR.
     *
     * @details If environment variable NO_COLOR is set to a non-empty variable,
     *          The color terminal is disabled.
     *          View https://no-color.org for more information.
     */
    static void detectEnv();
};

template <typename T>
inline void Term::error(const T& msg) {
    std::cerr << Term::ERR << "[ERROR] " << msg << Term::RESET << std::endl;
}

template <typename T>
inline void Term::message(const T& msg) {
    std::cerr << Term::MSG << "[MESSAGE] " << msg << Term::RESET << std::endl;
}

template <typename T>
inline void Term::warning(const T& msg) {
    std::cerr << Term::WAR << "[WARNING] " << msg << Term::RESET << std::endl;
}

#endif
