/**
 * @file read.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of class YAML_Error and ReadConfig
 * @version 0.1.0
 * @date 2022-07-12
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
    } catch (const YAML::ParserException& e) {
        // Note that this count starts from 0.
        YAML_Error err(e.msg, e.mark.line, e.mark.column);
        errors.push_back(err);
        return { config, errors }; // already loading error, just return error
    }
    YAML::Node _compiled;
    if (auto ver = config["version"]; ver.IsDefined()) {
        std::string ver_str = ver.as<std::string>();
        std::replace(ver_str.begin(), ver_str.end(), '.', ','); // 0.1.0 -> 0,1,0
        try {
            _compiled["versions"] = YAML::Load(std::string("[") + ver_str + "]"); // [0,1,0]
        } catch(...) {
            return {config, errors};
        }
        if (_compiled["versions"].size() == 3) {
            _compiled["version"] = _compiled["versions"][0].as<int>() * 0x10000
                                 + _compiled["versions"][1].as<int>() * 0x100
                                 + _compiled["versions"][2].as<int>(); // 0x000100
            config["_compiled"] = _compiled;
        } else {
            YAML_Error err(Err::VERSION_STRING_ERROR);
            errors.push_back(err);
            return { config, errors };
        }
    } else {
        YAML_Error err(Err::VERSION_NOT_SPECIFIED);
        errors.push_back(err);
        _compiled["versions"].push_back(_MMCESIM_VER_MAJOR);
        _compiled["versions"].push_back(_MMCESIM_VER_MINOR);
        _compiled["versions"].push_back(_MMCESIM_VER_REVISION);
        _compiled["version"] = _MMCESIM_VER;
        config["_compiled"] = _compiled;
    }
    std::cout << config << std::endl;
    return { config, errors };
}

std::tuple<YAML::Node, YAML_Errors> ReadConfig::read(std::string& file) {
    ReadConfig c(file);
    return c.read();
}
