/**
 * @file alg.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of class Alg
 * @version 0.1.0
 * @date 2022-07-25
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include "export/alg.h"

Alg::Alg(const std::string& str, bool fail_fast) {
    std::stringstream ss(str);
    std::string line;
    std::string unterminated_line = "";
    Alg_Lines::size_type line_no = 0;
    bool failed = false;
    while (std::getline(ss, line, '\n')) { // separate by newline
        ++line_no;
        trim(line); // remove the whitespace around it.
        if (line.empty()) continue; // skip empty line
        // bool unterminated_here = *(line.end() - 1) == '\\';
        if (*(line.end() - 1) == '\\') {
            // The last character is backslash
            // so the line has not terminated.
            unterminated_line += std::string(" ") + line;
        } else {
            try {
                Alg_Line l(unterminated_line + line);
                if (!failed) {
                    // If the status of 'failed' is true,
                    // the only aim of keeping parsing these lines
                    // is to check the syntax.
                    // No matter what happens later,
                    // the lines are not useable.
                    // Therefore, we only push line when we have not failed.
                    _lines.push_back(l);
                }
            } catch (const std::runtime_error& e) {
                failed = true;
                _errors.push_back({e.what(), unterminated_line + line, line_no});
            }
            unterminated_line.clear();
        }
    }
}
