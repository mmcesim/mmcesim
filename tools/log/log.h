/**
 * @file log.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief mmCEsim Log Tool
 * @version 0.2.0
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _MMCESIM_LOG_LOG_H_
#define _MMCESIM_LOG_LOG_H_

#include "meta.h"
#include "utils.h"
#include <clip.h>
#include <filesystem>
#include <fstream>
#include <iostream>

enum MMCESIM_LOG_ERR { CANNOT_OPEN_LOG_FILE = 100 };

static inline std::string logFileName() { return appDir() + "/mmcesim.log"; }

/**
 * @brief Get the log file name on the local machine.
 *
 * @return (std::string) The local log file name.
 */
static inline std::string localLogFileName() {
    std::filesystem::path p(logFileName());
    return p.string();
}

static inline void printLog() {
    std::ifstream f(logFileName());
    if (!f.is_open()) {
        // std::cerr << "[ERROR] Cannot open log file at '" << localLogFileName() << "'." << std::endl;
        // exit(MMCESIM_LOG_ERR::CANNOT_OPEN_LOG_FILE);
        return;
    }
    std::stringstream buf;
    buf << f.rdbuf();
    std::cout << buf.str() << std::endl;
}

static inline void copyLogToClipboard() {
    std::ifstream f(logFileName());
    if (!f.is_open()) {
        // std::cerr << "[ERROR] Cannot open log file at '" << localLogFileName() << "'." << std::endl;
        // exit(MMCESIM_LOG_ERR::CANNOT_OPEN_LOG_FILE);
        return;
    }
    std::stringstream buf;
    buf << f.rdbuf();
    clip::set_text(buf.str());
}

#endif
