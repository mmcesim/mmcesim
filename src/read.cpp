/**
 * @file read.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of class YAML_Error and ReadConfig
 * @version 0.1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include "read.h"

YAML_Error::YAML_Error(const std::string& msg_, int line_, int col_)
    : msg(msg_), line(line_), col(col_), ec(Err::YAML_SYNTAX) {}

YAML_Error::YAML_Error(const std::string& msg_, Err ec_, int line_, int col_)
    : msg(msg_), ec(ec_), line(line_), col(col_) {}

YAML_Error::YAML_Error(Err ec_, int line_, int col_)
    : msg(errorMsg(ec_)), ec(ec_), line(line_), col(col_) {}

ReadConfig::ReadConfig(const std::string& file) : _file(file) {}

// still working on
std::tuple<YAML::Node, YAML_Errors> ReadConfig::read() const {
    YAML_Errors errors;
    YAML::Node config;
    try {
        config = YAML::LoadFile(_file);
        std::cout << config["version"].as<std::string>() << "\n";
    } catch (const YAML::ParserException& e) {
        // Note that this count starts from 0.
        YAML_Error err(e.msg, e.mark.line, e.mark.column);
        errors.push_back(err);
    }
    return {config, errors};
}

std::tuple<YAML::Node, YAML_Errors> ReadConfig::read(std::string& file) {
    ReadConfig c(file);
    return c.read();
}
