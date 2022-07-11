/**
 * @file error_code.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Error Codes
 * @version 0.1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

#include <string>
#include <type_traits>
#include <iostream>

/**
 * @brief Error Code
 * 
 * Each error has its corresponding error.
 * It is worth noting that SUCCESS is also listed here.
 */
enum class Err: int {
    SUCCESS = 0,
    CLI_OPTIONS = 100,
    NO_GUI,
    INPUT_NOT_EXISTS,
};

/**
 * @brief Return the integer of the error code from class Err.
 * 
 * @param e Error Code (class Err)
 * @return int (The error code.)
 */
static inline int errorCode(const Err& e) noexcept {
    return static_cast<int>(e);
}

/**
 * @brief Return the error message of the error code from class Err.
 * 
 * @param e Error code (class Err).
 * @return std::string (The error message.)
 */
static inline std::string errorMsg(const Err& e) noexcept {
    switch(e) {
    case Err::SUCCESS: return "Success!";
    case Err::CLI_OPTIONS: return "Command line options error.";
    case Err::NO_GUI: return "No GUI App.";
    case Err::INPUT_NOT_EXISTS: return "Input file does not exist.";
    default: return "Error!";
    }
}

/**
 * @brief Print error message and exit with exit code.
 * 
 * @param e Error code (class Err).
 */
static inline void errorExit(const Err& e) noexcept {
    std::cerr << "ERROR: " << errorMsg(e) << '\n';
    exit(errorCode(e));
}

/**
 * @brief Check if the error code corresponds to an error.
 * 
 * This is used to excluded Err::SUCCESS.
 * 
 * @param e Error code (class Err).
 * @return true This is not an error (i.e. Err::SUCCESS).
 * @return false This is a real error (i.e. other than Err::SUCCESS).
 */
static inline bool isError(const Err& e) noexcept {
    return e != Err::SUCCESS;
}

#endif
