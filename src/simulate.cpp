#include "simulate.h"

Simulate::Simulate(const Shared_Info& info) : _s_info(info) {}

int Simulate::simulate() const {
    std::cout << "\n";
    std::string app_dir = appDir();
    std::string line;
    boost::process::ipstream is; //reading pipe-stream
    boost::process::child compile_process(
        _s_info.src_compile_cmd,
        boost::process::std_out > boost::process::null, // ignore output
        boost::process::std_err > is                    // keep error message
    );
    while (compile_process.running() && std::getline(is, line) && !line.empty()) {
        std::cerr << line << "\n";
    }
    compile_process.wait();
    int e = compile_process.exit_code();
    if (e) std::cerr << "\nCompiling failed. Command: " << _s_info.src_compile_cmd << std::endl;
    return e;
}

int Simulate::simulate(const Shared_Info& info) {
    Simulate sim(info);
    return sim.simulate();
}
