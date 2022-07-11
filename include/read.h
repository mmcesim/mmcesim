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

struct YAML_Error {
    std::string loc;
    std::string msg;
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