/**
 * @file log.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Log Class
 * @version 0.2.0
 * @date 2023-03-10
 *
 * @copyright Copyright (c) 2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "log.h"

Log::Log() : _f(appDir() + "/.log"), _open(_f.is_open()) {
    if (_open) {
        // get the current time
        std::time_t curr_time   = std::time(nullptr);
        std::tm curr_tm         = *std::localtime(&curr_time);
        const char* time_format = "%F %T (UTC %z)";
        info() << "* Time     : " << std::put_time(&curr_tm, time_format) << "\n";
        info() << "* Version  : " << _MMCESIM_VER_STR << "\n";
        info() << "* System   : " << spy::operating_system << "-" << spy::architecture << "\n";
        info() << "* Compiler : " << spy::compiler << std::endl;
    } else {
        Term::warning("Cannot open the log file to write.");
    }
}

Log::~Log() {
    if (_open) _f.close();
}

Log _log;
