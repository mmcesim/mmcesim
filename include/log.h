/**
 * @file log.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Log Utilities
 * @version 0.2.0
 * @date 2023-03-17
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
     * @note If the log file cannot be opened, a warning will be displayed.
     */
    Log();

    /**
     * @brief Destroy the Log object.
     *
     * @details Close the ostream file.
     */
    ~Log();

    /**
     * @brief Write contents to the log file.
     *
     * @return (std::ofstream&) The output (log) file stream.
     */
    std::ofstream& write();

    /**
     * @brief Write infomation to the log file.
     *
     * @details The line is started with [INFO]
     * @return (std::ofstream&) The output (log) file stream.
     */
    std::ofstream& info();

    /**
     * @brief Write warning message to the log file.
     *
     * @details The line is started with [WARNING]
     * @return (std::ofstream&) The output (log) file stream.
     */
    std::ofstream& war();

    /**
     * @brief Write error message to the log file.
     *
     * @details The line is started with [ERROR]
     * @return (std::ofstream&) The output (log) file stream.
     */
    std::ofstream& err();

    /**
     * @brief Write a message to the log file.
     *
     * @details The line is started with [MESSAGE]
     * @return (std::ofstream&) The output (log) file stream.
     */
    std::ofstream& msg();

    /**
     * @brief Flush the buff into the log file.
     *
     * @details This is equivalent to Log::write() << std::endl.
     */
    void flush();

    /**
     * @brief Write CLI arguments information to log.
     *
     * @param argc The number of CLI arguments.
     * @param argv The CLI arguments.
     */
    void writeArg(int argc, char* argv[]);

  private:
    std::ofstream _f;   ///< output file stream
    bool _open = false; ///< whether the file successfully opened
};

inline std::ofstream& Log::write() { return _f; }

inline std::ofstream& Log::info() {
    _f << "[INFO] ";
    return _f;
}

inline std::ofstream& Log::war() {
    _f << "[WARNING] ";
    return _f;
}

inline std::ofstream& Log::err() {
    _f << "[ERROR] ";
    return _f;
}

inline std::ofstream& Log::msg() {
    _f << "[MESSAGE] ";
    return _f;
}

inline void Log::flush() { _f << std::flush; }

#endif
