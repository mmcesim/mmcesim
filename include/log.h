/**
 * @file log.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Log Utilities
 * @version 0.2.0
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _LOG_H_
#define _LOG_H_

#include "meta.h"
#include "term.h"
#include "utils.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <spy.hpp>

/**
 * @brief Log support during compilation.
 *
 */
class Log {

  public:
    /**
     * @brief Construct a new Log object.
     *
     * @details The file initialization is processed here.
     */
    Log();

    /**
     * @brief Destroy the Log object.
     *
     * @details Close the ostream file.
     */
    ~Log();

    std::ofstream& info();

    std::ofstream& war();

    std::ofstream& msg();

  private:
    std::ofstream _f;   ///< output file stream
    bool _open = false; ///< whether the file successfully opened
};

inline std::ofstream& Log::info() {
    _f << "[INFO] ";
    return _f;
}

inline std::ofstream& Log::war() {
    _f << "[WARNING] ";
    return _f;
}

inline std::ofstream& Log::msg() {
    _f << "[MESSAGE] ";
    return _f;
}

#endif
