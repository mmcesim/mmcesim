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
#include "cli_options.h"
#include "error_code.h"
#include "meta.h"
#include "utils.h"
#include "simulate.h"
#include "export.h"
#include "style.h"
#include "config.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 4244)
#endif

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
        ("style,s", po::value<std::string>(&opt.style),
            "style options (C++ only, with astyle)")
        ("lang,l", po::value<std::string>(&opt.lang),
            "export language or simulation backend")
        ("value", po::value<std::string>(&opt.value),
            "value for configuration option")
        ("force,f", "force writing mode")
        ("no-error-compile", "Do not raise error if simulation compiling fails")
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
                      << "  config                 configure mmCEsim options\n"
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
            if (!std::filesystem::exists(gui_path) &&
                !std::filesystem::exists(gui_path + ".out") &&
                !std::filesystem::exists(gui_path + ".exe")) {
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

    if (vm.count("force")) opt.force = true;
    if (vm.count("no-error-compile")) opt.no_error_compile = true;

    if (opt.cmd != "config" && !std::filesystem::exists(opt.input)) {
        opt.input += ".sim";
        if (!std::filesystem::exists(opt.input)) errorExit(Err::INPUT_NOT_EXISTS);
    }
    boost::algorithm::to_lower(opt.cmd);
    if (opt.cmd == "sim" || opt.cmd == "simulate") {
        Shared_Info info;
        auto&& errors = Export::exportCode(opt, &info);
        if (hasError(errors)) errorExit(errors[0].ec); // TODO: should distinguish error and warning
        if (int compile_result = Simulate::simulate(info)) {
            if (opt.no_error_compile) {
                std::cout << "Compiling Error with exit code " << compile_result << ".\n";
            } else {
                std::cerr << "ERROR: Simulation compiling error. Compiling exit with code " << compile_result << ".\n";
                return errorCode(Err::COMPILE_ERROR);
            }
        }
    } else if (opt.cmd == "exp" || opt.cmd == "export") {
        auto&& errors = Export::exportCode(opt);
        if (hasError(errors)) {
            for (auto&& err : errors) {
                std::cerr << err.msg << '\n';
            }
            errorExit(errors[0].ec);
        }
        if (int astyle_result = Style::style(opt.output, opt.style); astyle_result) {
            std::cerr << "ERROR: Formatting error. Astyle exit with code " << astyle_result << ".\n";
            return errorCode(Err::ASTYLE_ERROR);
        }
    } else if (opt.cmd == "config") {
        if (vm.count("value")) {
            std::string msg;
            if (!Config::edit(opt.input, opt.value, &msg)) {
                std::cerr << "ERROR: " << msg << std::endl;
                return errorCode(Err::CONFIG_ERROR);
            }
        } else {
            std::cout << Config::read(opt.input) << std::endl;
        }
        return 0;
    } else {
        errorExit(Err::UNKOWN_CMD);
    }

    return 0;
}
