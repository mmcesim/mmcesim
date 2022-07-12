/**
 * @file read.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Read YAML Configuration
 * @version 0.1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _READ_H_
#define _READ_H_

#define YAML_CPP_STATIC_DEFINE

#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include "error_code.h"
#include "meta.h"

struct YAML_Error {
    int line = -1;
    int col = -1;
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

    static std::tuple<YAML::Node, YAML_Errors> read(std::string& file);

private:
    std::string _file;
};

#endif