/**
 * @file maintain.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief mmCEsim Maintainence Tool
 * @version 0.2.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "maintain.h"
#include "_boost_config.h"
#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>

#ifdef _MSC_VER
#    define _CRT_SECURE_NO_WARNINGS
#    pragma warning(disable : 4244)
#endif

int main(int argc, char* argv[]) {
    namespace po = boost::program_options;

    po::options_description generic("Generic options");
    // clang-format off
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
    ;
    // clang-format on

    po::options_description config("Configuration");
    config.add_options()("latest,l", "Get latest upstream mmCEsim version");

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config);

    po::options_description config_file_options;
    config_file_options.add(config);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    po::positional_options_description p;

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
        if (vm.count("help")) {
            std::cout << _MMCESIM_NAME << ' ' << _MMCESIM_VER_STR << "  (C) 2022-2023 " << _MMCESIM_AUTHOR << '\n'
                      << _MMCESIM_DESCR << std::endl;
            std::cout << std::string(45, '=') << std::endl;
            std::cout << "This is the maintain tool.\n" << std::endl;
            std::cout << "Usage: " << argv[0] << " [options]\n" << std::endl;
            std::cout << visible << std::endl;
            return 0;
        }

        if (vm.count("version")) {
            std::cout << _MMCESIM_NAME << "-maintain " << _MMCESIM_VER_STR << std::endl;
            return 0;
        }

        if (vm.count("latest")) {
            std::string s = fetchUpstreamVersion();
            std::cout << "Latest version: " << s << std::endl;
        }

        // po::notify(vm);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Use '" << argv[0] << " -h' for help." << std::endl;
        return 1;
    }

    return 0;
}
