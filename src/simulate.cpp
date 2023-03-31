/**
 * @file simulate.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Simulate Class
 * @version 0.2.1
 * @date 2023-03-17
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "simulate.h"

Simulate::Simulate(const Shared_Info& info) : _s_info(info) {}

int Simulate::simulate() const {
    std::cout << "\n";
    std::string app_dir = appDir();
    std::string line;
    std::string cmd = fmt::format(_s_info.src_compile_cmd, Config::read("cpp", "g++"), Config::read("cppflags"));
    _log.info() << "Simulation CMD: " << cmd << std::endl;
    try {
        boost::process::ipstream is; // reading pipe-stream
        boost::process::child compile_process(cmd,
                                              boost::process::std_out > boost::process::null, // ignore output
                                              boost::process::std_err > is                    // keep error message
        );
        while (compile_process.running() && std::getline(is, line) && !line.empty()) { std::cerr << line << "\n"; }
        compile_process.wait();
        int e = compile_process.exit_code();
        if (e) {
            std::cerr << "\nCompiling failed. Command: " << cmd << std::endl;
            _log.err() << "Compiling failed." << std::endl;
            return e;
        } else {
            std::cout << "[mmcesim] simulate $ Code auto export finished." << std::endl;
            boost::process::ipstream is; // reading pipe-stream
            boost::process::child simulate_process("./a.out",
                                                   boost::process::std_out > boost::process::null, // ignore output
                                                   boost::process::std_err > is                    // keep error message
            );
            while (simulate_process.running() && std::getline(is, line) && !line.empty()) { std::cerr << line << "\n"; }
            simulate_process.wait();
            int e = simulate_process.exit_code();
            if (e) {
                std::cerr << "\nSimulation running failed. Command: ./a.out" << std::endl;
                _log.err() << "Simulation running failed. Command: ./a.out" << std::endl;
                return e;
            } else {
                std::cout << "[mmcesim] simulate $ Simulation succeeded." << std::endl;
                _log.info() << "Simulation succeeded." << std::endl;
                return 0;
            }
        }
    } catch (const boost::process::process_error& e) {
        std::cerr << "\nCompiling failed. Command: " << cmd << std::endl;
        _log.err() << "Compiling failed." << std::endl;
        return -1;
    }
}

int Simulate::simulate(const Shared_Info& info) {
    Simulate sim(info);
    return sim.simulate();
}
