/**
 * @file style.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Style class.
 * @version 0.3.0
 * @date 2022-07-17
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "style.h"

Style::Style(const std::string& file_name, const std::string& options) : _file_name(file_name), _options(options) {}

int Style::style() const {
    std::cout << "\n";
    std::string app_dir     = appDir();
    std::string astyle_path = app_dir.append("/astyle");
    // --indent-namespaces / -N
    // --indent-col1-comments / -Y
    // --pad-oper / -p
    // --pad-comma / -xg
    // --pad-header / -H
    std::string astyle_options = std::string("--suffix=none -N -Y -p -xg -H ") + _options + " " + _file_name;
    auto astyle_cmd            = astyle_path + " " + astyle_options;
    _log.info() << "Astyle CMD: " << astyle_cmd << std::endl;
    std::string line;
    boost::process::ipstream is; // reading pipe-stream
    boost::process::child astyle_process(astyle_cmd,
                                         boost::process::std_out > boost::process::null, // ignore output
                                         boost::process::std_err > is                    // keep error message
    );
    while (astyle_process.running() && std::getline(is, line) && !line.empty()) { std::cerr << line << "\n"; }
    astyle_process.wait();
    int e = astyle_process.exit_code();
    if (e) {
        std::cerr << Term::ERR << "\n[ERROR] Internal astyle command failed: " << astyle_cmd << Term::RESET
                  << std::endl;
        _log.err() << "Astyle failed with exit code " << e << "." << std::endl;
    } else _log.info() << "Astyle finished successfully." << std::endl;
    return e;
}

int Style::style(const std::string& file_name, const std::string& options) {
    Style s(file_name, options);
    return s.style();
}
