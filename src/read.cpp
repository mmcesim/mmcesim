/**
 * @file read.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of class ReadConfig
 * @version 0.1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include "read.h"

ReadConfig::ReadConfig(const std::string& file) : _file(file) {}

// still working on
std::tuple<YAML::Node, YAML_Errors> ReadConfig::read() const {
    YAML::Node config = YAML::LoadFile(_file);
    std::cout << config["version"].as<std::string>() << "\n";
    YAML_Errors errors;
    return {config, errors};
}

std::tuple<YAML::Node, YAML_Errors> ReadConfig::read(std::string& file) {
    ReadConfig c(file);
    return c.read();
}
