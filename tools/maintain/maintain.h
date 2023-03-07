/**
 * @file maintain.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief mmCEsim Maintainence Functions
 * @version 0.2.0
 * @date 2023-03-07
 *
 * @copyright Copyright (c) 2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "meta.h"
#include <boost/process.hpp>
#include <fstream>
#include <iostream>

/**
 * @brief Fetch the mmCEsim upstream version.
 *
 * @details This function uses 'curl' to obtain the contents of "https://mmcesim.org/VERSION".
 * @return (std::string) The latest version string.
 */
static inline std::string fetchUpstreamVersion() {
    const std::string url = "https://mmcesim.org/VERSION";
    std::string response;
    std::string error;
    boost::process::ipstream out, err;
    try {
        boost::process::child curl_process("curl " + url, boost::process::std_out > out, boost::process::std_err > err);
        while (curl_process.running() && (std::getline(out, response) || std::getline(err, error))) {
            if (!error.empty()) std::cerr << error << "\n";
            else return response;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return "";
    }

    return response;
}
