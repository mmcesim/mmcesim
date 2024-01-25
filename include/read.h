/**
 * @file read.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Read YAML Configuration
 * @version 0.3.0
 * @date 2022-07-11
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _READ_H_
#define _READ_H_

#include "error_code.h"
#include "meta.h"
#include "yaml.h"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

struct YAML_Error {
    int line = -1;
    int col  = -1;
    std::string msg;
    Err ec;

    YAML_Error(const std::string& msg, int line = -1, int col = -1);

    YAML_Error(const std::string& msg, Err ec, int line = -1, int col = -1);

    YAML_Error(Err ec, int line = -1, int col = -1);
};

using YAML_Errors = std::vector<YAML_Error>;

class ReadConfig {
  public:
    ReadConfig(const std::string& file);

    std::tuple<YAML::Node, YAML_Errors> read() const;

    static std::tuple<YAML::Node, YAML_Errors> read(const std::string& file);

  private:
    std::string _file;
};

static inline bool hasError(const YAML_Errors& e) {
    for (auto&& item : e) {
        if (isError(item.ec)) return true;
    }
    return false;
}

#endif