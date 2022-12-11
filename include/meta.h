/**
 * @file meta.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief mmCEsim meta data
 * @version 0.1.0
 * @date 2022-07-11
 *
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _META_H_
#define _META_H_

#include <string>

const std::string _MMCESIM_NAME    = "mmCEsim";
const std::string _MMCESIM_DESCR   = "Millimeter Wave Channel Estimation Simulation";
const std::string _MMCESIM_WEB     = "https://mmcesim.org";
const std::string _MMCESIM_BLOG    = "https://blog.mmcesim.org";
const std::string _MMCESIM_WEBAPP  = "https://app.mmcesim.org";
const std::string _MMCESIM_GIT     = "https://github.com/mmcesim";
const std::string _MMCESIM_TWI     = "https://twitter.com/mmcesim";
const std::string _MMCESIM_EMAIL   = "contact@mmcesim.org";
const std::string _MMCESIM_AUTHOR  = "Wuqiong Zhao";
const std::string _MMCESIM_LIC     = "MIT License";
const int _MMCESIM_VER_COMP        = 0x000100; // compatibility: 0.1.0
const int _MMCESIM_VER_MAJOR       = 0;
const int _MMCESIM_VER_MINOR       = 1;
const int _MMCESIM_VER_REVISION    = 0;
const int _MMCESIM_VER             = 0x10000 * _MMCESIM_VER_MAJOR + 0x100 * _MMCESIM_VER_MINOR + _MMCESIM_VER_REVISION;
const std::string _MMCESIM_VER_STR = std::to_string(_MMCESIM_VER_MAJOR) + "." + std::to_string(_MMCESIM_VER_MINOR) +
                                     "." + std::to_string(_MMCESIM_VER_REVISION);

#endif
