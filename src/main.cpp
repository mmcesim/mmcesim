/**
 * @file main.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Program Command Line Options
 * @version 0.1.0
 * @date 2022-07-11
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>
#include "cli_options.h"
#include "error_code.h"
#include "meta.h"
#include "utils.h"
#include "simulate.h"

int main(int argc, char* argv[]) {
    namespace po = boost::program_options;

    CLI_Options opt; // command line options

    po::options_description generic("Generic options");
    generic.add_options()
        ("version,v", "print version string")
        ("help,h", "produce help message")
        ("gui", "open the GUI app")
    ;

    po::options_description config("Configuration");
    config.add_options()
        ("output,o", po::value<std::string>(&opt.output),
            "output file name")
    ;

    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("command", po::value<std::string>(&opt.cmd)->required(), "command")
        ("input", po::value<std::string>(&opt.input)->required(), "input")
    ;

    po::options_description cmdline_options;
    cmdline_options.add(generic).add(config).add(hidden);

    po::options_description config_file_options;
    config_file_options.add(config).add(hidden);

    po::options_description visible("Allowed options");
    visible.add(generic).add(config);

    po::positional_options_description p;
    p.add("command", 1);
    p.add("input", -1);
    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).
            options(cmdline_options).positional(p).run(), vm);
        if (vm.count("help")) {
            std::cout << _MMCESIM_NAME << ' ' << _MMCESIM_VER_STR
                      << "    (C) " << _MMCESIM_AUTHOR << '\n'
                      << _MMCESIM_DESCR << std::endl;
            std::cout << std::string(45, '=') << "\n" << std::endl;
            std::cout << "Usage: " << argv[0] << " <command> <input> [options]\n" << std::endl;
            std::cout << "Commands:\n"
                      << "  sim [simulate]         run simulation\n"
                      << "  dbg [debug]            debug simulation settings\n"
                      << "  exp [export]           export code\n"
                      << "  fmt [format]           format .sim configuration file\n"
                      << "  (Leave empty)          generic use\n" << std::endl;
            std::cout << visible << std::endl;
            return 0;
        }

        if (vm.count("version")) {
            std::cout << _MMCESIM_NAME << ' ' << _MMCESIM_VER_STR << std::endl;
            return 0;
        }

        if (vm.count("gui")) {
            std::string app_dir = appDir();
            std::string gui_path = app_dir.append("/mmcesim-gui");
#ifdef __APPLE__
            // open GUI app without handling its result
            if (!std::filesystem::exists(gui_path + ".app")) {
                std::cerr << "Error: " << errorMsg(Err::NO_GUI) << std::endl;
                return errorCode(Err::NO_GUI);
            } else {
                std::cout << "Opening GUI application." << std::endl;
                boost::process::spawn(std::string("open ") + gui_path + ".app");
            }
#else
            // open GUI app without handling its result
            if (!std::filesystem::exists(gui_path)) {
                std::cerr << "Error: " << errorMsg(Err::NO_GUI) << std::endl;
                return errorCode(Err::NO_GUI);
            } else {
                std::cout << "Opening GUI application." << std::endl;
                boost::process::spawn(gui_path);
            }
#endif

            return 0; // open GUI app
        }
    
        po::notify(vm);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Use '" << argv[0] << " -h' for help." << std::endl;
        return errorCode(Err::CLI_OPTIONS);
    }

    if (!std::filesystem::exists(opt.input)) {
        errorExit(Err::INPUT_NOT_EXISTS);
    }
    boost::algorithm::to_lower(opt.cmd);
    if (opt.cmd == "sim" || opt.cmd == "simulate") {
        auto [config, errors] = ReadConfig::read(opt.input);
        if (!errors.empty()) errorExit(errors[0].ec);
    } else {
        // other things
    }

    return 0;
}
