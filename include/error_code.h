/**
 * @file error_code.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Error Codes (Including Warnings)
 * @version 0.1.0
 * @date 2022-07-12
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

#include <string>
#include <type_traits>
#include <iostream>
#include <cassert>
#include "meta.h"

/**
 * @brief Error Code
 * 
 * Each error has its corresponding error.
 * It is worth noting that SUCCESS is also listed here.
 */
enum class Err: int {
    SUCCESS = 0,
    // error
    CLI_OPTIONS = 100,
    NO_GUI,
    INPUT_NOT_EXISTS,
    UNKOWN_CMD,
    OUTPUT_FILE_EXISTS,
    CANNOT_OPEN_OUTPUT_FILE,
    YAML_SYNTAX,
    YAML_DTYPE,
    VERSION_STRING_ERROR,
    // warning
    VERSION_NOT_SPECIFIED = 200,
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
    case Err::UNKOWN_CMD: return "Unknown command (use mmcesim -h for more information).";
    case Err::OUTPUT_FILE_EXISTS: return "Output file exists. To force overwriting, you can use option '--force' or '-f'.";
    case Err::CANNOT_OPEN_OUTPUT_FILE: return "Cannot open output file.";
    case Err::YAML_SYNTAX: return "YAML syntax error.";
    case Err::YAML_DTYPE: return "YAML data type error.";
    case Err::VERSION_STRING_ERROR: return "Invalid version string.";
    case Err::VERSION_NOT_SPECIFIED: return "Version string not specified. Assume as application version " + _MMCESIM_VER_STR + ".";
    default: return "Error!";
    }
}

/**
 * @brief Check if the error code corresponds to an error.
 * 
 * @param e Error code (class Err).
 * @return true This is not an error.
 * @return false This is a real error.
 */
static inline bool isError(const Err& e) noexcept {
    return errorCode(e) >= 100 && errorCode(e) < 200;
}

/**
 * @brief Check if the error code corresponds to a warning.
 * 
 * @param e Error code (class Err).
 * @return true This is not a warning.
 * @return false This is a real warning.
 */
static inline bool isWarning(const Err& e) noexcept {
    return errorCode(e) >= 200;
}

/**
 * @brief Check if the error code corresponds to a warning.
 * 
 * This is used to check if it is Err::SUCCESS.
 * 
 * @param e Error code (class Err).
 * @return true This is success.
 * @return false This is not success.
 */
static inline bool isSuccess(const Err& e) noexcept {
    return e == Err::SUCCESS;
}

/**
 * @brief Print error message and exit with exit code.
 * 
 * @param e Error code (class Err).
 */
static inline void errorExit(const Err& e) noexcept {
    assert((isError(e) && "check if it is really an error in errorExit"));
    std::cerr << "ERROR: " << errorMsg(e) << '\n';
    exit(errorCode(e));
}

#endif
