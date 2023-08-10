/**
 * @file style.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Format Output File
 * @version 0.2.2
 * @date 2022-07-17
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _STYLE_H_
#define _STYLE_H_

#include <iostream>
#include <string>
#pragma GCC diagnostic ignored "-Wc++11-narrowing"
#pragma GCC diagnostic push
#include <boost/process.hpp>
#pragma GCC diagnostic pop
#include "error_code.h"
#include "utils.h"

/**
 * @brief Style (format) generated code.
 *
 * @details This class will call child process from 'astyle'.
 */
class Style {
  public:
    Style(const std::string& file_name, const std::string& options);

    /**
     * @brief Call the astyle child process to style the generated C++ code.
     *
     * @return (int) Exit code of 'astyle'.
     */
    int style() const;

    /**
     * @brief Call the astyle child process to style the generated C++ code.
     *
     * @param file_name The generated C++ file name to be formatted.
     * @param options The 'astyle' format options.
     * @return (int) Exit code of 'astyle'.
     */
    static int style(const std::string& file_name, const std::string& options);

  private:
    std::string _file_name; ///< The generated C++ file name to be formatted.
    std::string _options;   ///< The 'astyle' format options.
};

#endif
