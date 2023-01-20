/**
 * @file maintain.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief mmCEsim Maintainence Tool
 * @version 0.2.0
 * @date 2023-01-20
 *
 * @copyright Copyright (c) 2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "meta.h"
#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>
#include <iostream>

#ifdef _MSC_VER
#    define _CRT_SECURE_NO_WARNINGS
#    pragma warning(disable : 4244)
#endif

int main(int argc, char* argv[]) {
    namespace po = boost::program_options;

    po::options_description generic("Generic options");
    generic.add_options()("version,v", "print version string")("help,h", "produce help message");

    std::cout << _MMCESIM_VER_STR << std::endl;
    return 0;
}
