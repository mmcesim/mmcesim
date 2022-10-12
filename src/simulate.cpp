#include "simulate.h"

Simulate::Simulate(const Shared_Info& info) : _s_info(info) {}

int Simulate::simulate() const {
    std::cout << "\n";
    std::string app_dir = appDir();
    std::string line;
    std::string cmd = fmt::format(_s_info.src_compile_cmd, Config::read("cpp", "g++"), Config::read("cppflags"));
    try {
        boost::process::ipstream is; //reading pipe-stream
        boost::process::child compile_process(
            cmd,
            boost::process::std_out > boost::process::null, // ignore output
            boost::process::std_err > is                    // keep error message
        );
        while (compile_process.running() && std::getline(is, line) && !line.empty()) {
            std::cerr << line << "\n";
        }
        compile_process.wait();
        int e = compile_process.exit_code();
        if (e) std::cerr << "\nCompiling failed. Command: " << cmd << std::endl;
        return e;
    } catch (const boost::process::process_error& e) {
        std::cerr << "\nCompiling failed. Command: " << cmd << std::endl;
        return -1;
    }
    
}

int Simulate::simulate(const Shared_Info& info) {
    Simulate sim(info);
    return sim.simulate();
}
