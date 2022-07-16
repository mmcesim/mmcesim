/**
 * @file style.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Format Output File
 * @version 0.1.0
 * @date 2022-07-17
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _STYLE_H_
#define _STYLE_H_

#include <iostream>
#include <string>
#include <boost/process.hpp>
#include "error_code.h"
#include "utils.h"

class Style {
public:

    Style(const std::string& file_name, const std::string& options);

    /**
     * @brief Call the astyle child process to style the generated Verilog code.
     * 
     * @return int (Exit code of 'astyle'.)
     */
    int style() const;

    static int style(const std::string& file_name, const std::string& options);

private:
    std::string _file_name;
    std::string _options;
};

#endif
