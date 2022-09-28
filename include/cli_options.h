/**
 * @file cli_options.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Storage of Command Line Options
 * @version 0.1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _CLI_OPTIONS_H_
#define _CLI_OPTIONS_H_

#include <string>

struct CLI_Options {
    std::string cmd;
    std::string input;
    std::string output = "__UNDEFINED";
    std::string style;
    std::string lang;
    std::string value;
    bool force = false;
};

#endif