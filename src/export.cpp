#include "export.h"

Export::Export(CLI_Options& opt) : _opt(opt) {
    std::tie(_config, _errors) = ReadConfig::read(opt.input);
    if (hasError(_errors)) {
        // The reading process already has errors not warnings,
        // so there is no need to export.
        _already_error_before_export = true;
        return;
    }
    _setLang();
    if (_opt.output == "__UNDEFINED") {
        std::filesystem::path input_path(_opt.input);
        _opt.output = input_path.replace_extension(_langExtension()).string();
        if (!_opt.force && !std::filesystem::exists(_opt.output)) {
            YAML_Error e(Err::OUTPUT_FILE_EXISTS);
            _errors.push_back(e);
            _already_error_before_export = true;
        }
    }
    _f_ptr = new std::ofstream(_opt.output);
    if (!_f().is_open()) {
        YAML_Error e(Err::CANNOT_OPEN_OUTPUT_FILE);
        _errors.push_back(e);
        _already_error_before_export = true;
    }
}

Export::Export(CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors) 
    : _opt(opt), _config(config), _errors(errors) {
    if (hasError(_errors)) {
        // The reading process already has errors not warnings,
        // so there is no need to export.
        _already_error_before_export = true;
        return;
    }
    _setLang();
    if (_opt.output == "__UNDEFINED") {
        std::filesystem::path input_path(_opt.input);
        _opt.output = input_path.replace_extension(_langExtension()).string();
        if (!_opt.force && !std::filesystem::exists(_opt.output)) {
            YAML_Error e(Err::OUTPUT_FILE_EXISTS);
            _errors.push_back(e);
            _already_error_before_export = true;
        }
    }
    _f_ptr = new std::ofstream(_opt.output);
    if (!_f().is_open()) {
        YAML_Error e(Err::CANNOT_OPEN_OUTPUT_FILE);
        _errors.push_back(e);
        _already_error_before_export = true;
    }
}

Export::~Export() {
    // need to check if _f_ptr has been initialized
    // since if there is YAML reading error,
    // the file stream pointer is never initialized.
    if (_f_ptr) delete _f_ptr;
}

YAML_Errors Export::exportCode() {
    if (_already_error_before_export) {
        _info("Error before executing exporting.");
        return _errors;
    }
    // do something
    _topComment();
    _beginning();
    _setTransmitterReceiver();
    _setMaxTestNum();
    _setVarNames();
    _generateChannels();
    _algorithms();
    _sounding();
    // _estimation();
    _reporting();
    _ending();
    _f().close();
    return _errors;
}

YAML_Errors Export::exportCode(CLI_Options& opt) {
    Export ep(opt);
    return ep.exportCode();
}

YAML_Errors Export::exportCode(CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors) {
    Export ep(opt, config, errors);
    return ep.exportCode();
}

bool Export::_preCheck(const YAML::Node& n, unsigned a_t, bool mattered) {
    bool find_match = false;
    if (a_t & DType::INT && n.IsScalar()) {
        find_match = true;
        try { n.as<int>(); } catch(...) { find_match = false; }
    }
    if (!find_match && a_t & DType::DOUBLE && n.IsScalar()) {
        find_match = true;
        try { n.as<double>(); } catch(...) { find_match = false; }
    }
    if (!find_match && a_t & DType::STRING && n.IsScalar()) {
        find_match = true;
        try { n.as<std::string>(); } catch(...) { find_match = false; }
    }
    if (!find_match && a_t & DType::BOOL && n.IsScalar()) {
        find_match = true;
        try { n.as<bool>(); } catch(...) { find_match = false; }
    }
    if (!find_match && a_t & DType::CHAR && n.IsScalar()) {
        find_match = true;
        try { n.as<char>(); } catch(...) { find_match = false; }
    }
    if (!find_match && a_t & DType::SEQ && n.IsSequence()) {
        find_match = true;
    }
    if (!find_match && a_t & DType::MAP && n.IsMap()) {
        find_match = true;
    }
    if (!find_match && a_t & DType::NUL && n.IsNull()) {
        find_match = true;
    }
    if (!find_match && a_t & DType::UNDEF && !n.IsDefined()) {
        find_match = true;
    }
    if (find_match) return true;
    else {
        if (mattered) {
            // This is the general error message,
            // but in use, the error message can be more specific
            // by using method _setLatestError.
            std::string msg = errorMsg(Err::YAML_DTYPE) + " (" + n.Tag() + ")";
            YAML_Error e(msg, Err::YAML_DTYPE);
            _errors.push_back(e);
        }
        return false;
    }
}

// TODO: If backend in configuration disagrees with the command line option, raise a warning.
void Export::_setLang() {
    _info(std::string("This is mmCEsim ") + _MMCESIM_VER_STR
        + ", with target version " + _config["_compiled"]["version_str"].as<std::string>() + ".");
    if (auto n = _config["simulation"]; _preCheck(n, DType::MAP)) {
        if (n["backend"].IsDefined()) {
            try {
                std::string lang_str = _as<std::string>(n["backend"]);
                boost::algorithm::to_lower(lang_str);
                if (lang_str == "cpp" || lang_str == "c++") lang = Lang::CPP;
                if (lang_str == "matlab" || lang_str == "m") lang = Lang::MATLAB;
                if (lang_str == "octave" || lang_str == "gnu octave") lang = Lang::OCTAVE;
                if (lang_str == "py" || lang_str == "python") lang = Lang::PY;
                if (lang_str == "ipynb" || lang_str == "jupyter") lang = Lang::IPYNB;
            } catch(...) {
                _setLatestError("'simulation->backend' is not a string"
                    " (should be \"cpp\", \"matlab\", \"octave\", \"py\" or \"ipynb\").");
                return;
            }
        } else if (std::string lang_str = _opt.lang; lang_str != "") {
            boost::algorithm::to_lower(lang_str);
            if (lang_str == "cpp" || lang_str == "c++") lang = Lang::CPP;
            if (lang_str == "matlab" || lang_str == "m") lang = Lang::MATLAB;
            if (lang_str == "octave" || lang_str == "gnu octave") lang = Lang::OCTAVE;
            if (lang_str == "py" || lang_str == "python") lang = Lang::PY;
            if (lang_str == "ipynb" || lang_str == "jupyter") lang = Lang::IPYNB;
            else {
                YAML_Error e("Unrecognized language with command line option '--lang' or '-l'.", Err::CLI_OPTIONS);
                _errors.push_back(e);
                _already_error_before_export = true;
                return;
            }
        } else {
            std::filesystem::path path(_opt.output);
            lang_str = path.extension().string();
            if (lang_str == "cpp" || lang_str == "c++") lang = Lang::CPP;
            if (lang_str == "matlab" || lang_str == "m") lang = Lang::MATLAB;
            if (lang_str == "octave" || lang_str == "octave") lang = Lang::OCTAVE;
            if (lang_str == "py" || lang_str == "python") lang = Lang::PY;
            if (lang_str == "ipynb") lang = Lang::IPYNB;
            else {
                lang = Lang::CPP;
                _info("Language/backend not specified. Assumed as " + _langName() + ".");
                return;
            }
        }
    } else {
        _setLatestError("Simulation block is not defined or has wrong type.");
        _already_error_before_export = true;
        return;
    }
    _info("Set simulation backend as " + _langName() + ".");
}

std::tuple<bool, std::string, std::string> Export::_setChannelGains(const YAML::Node& n) {
    if (auto gains = n["gains"]; _preCheck(gains, DType::MAP)) {
        if (auto mode = boost::algorithm::to_lower_copy(_asStr(gains["mode"])); mode == "normal") {
            std::string mean = _asStr(gains["mean"]);
            std::string variance = _asStr(gains["variance"]);
            return { true, mean, variance };
        } else if (mode == "uniform") {
            std::string min = _asStr(gains["min"]);
            std::string max = _asStr(gains["max"]);
            return { false, min, max };
        }
    } else if (_preCheck(n["node"], DType::UNDEF)) {
        // default value if not specified
        return { true, "0", "1" };
    } else {
        throw("Channel gains not correctly set!");
    }
    return { true, "0", "1" };
}

void Export::_topComment() {
    // TODO: ipynb settings
    std::string title;
    try {
        title = _asStr(_config["meta"]["title"], false);
        if (title == "") throw("Title empty!");
    } catch(...) {
        title =  _opt.output ; // TODO: Only file name.
    }
    _wComment() << "Title: " << title << '\n';
    try {
        std::string desc = _asStr(_config["meta"]["description"], false);
        _wComment() << "Description: " << desc << '\n';
    } catch(...) {}
    try {
        std::string author = _asStr(_config["meta"]["author"], false);
        _wComment() << "Author: " << author << '\n';
    } catch(...) {}

    // get the current time
    std::time_t curr_time = std::time(nullptr);
    std::tm     curr_tm   = *std::localtime(&curr_time);
    const char* time_format = "%F %T (UTC %z)";

    _wComment() << "Date: " << std::put_time(&curr_tm, time_format) << "\n";
    _wComment() << '\n';
    _wComment() << "This file is auto generated using " << _MMCESIM_NAME << ' ' << _MMCESIM_VER_STR << ",\n";
    _wComment() << "With initial target as version " << _config["_compiled"]["version_str"] << ".\n";
    _wComment() << '\n';
    _wComment() << _MMCESIM_NAME << " is open source under the " << _MMCESIM_LIC << ".\n";
    _wComment() << "GitHub organization at " << _MMCESIM_GIT << ".\n";
    _wComment() << "Web app is available at " << _MMCESIM_WEBAPP << ".\n";
    _wComment() << "Visit " << _MMCESIM_WEB << " for more information.\n";
    if (lang == Lang::CPP) {
        _wComment() << '\n';
        _wComment() << "Compile Commands:\n";
        _wComment() << "$ g++ " << _opt.output << " -std=c++17 -larmadillo\n";
        _wComment() << "or\n";
        _wComment() << "$ clang++ " << _opt.output << " -std=c++17 -larmadillo\n";
        _wComment() << "or just link to Armadillo library with whatever compiler you have.\n";
    }
    _f() << "\n";
}

void Export::_beginning() {
    // load header
    std::ifstream header_file(appDir() + "/../include/mmcesim/copy/header." + _langMmcesimExtension());
    std::string header_content = "";
    while (!header_file.eof()) {
        header_content += header_file.get();
    }
    header_content.erase(header_content.end() - 1); // last read character is invalid, erase it
    _f() << header_content << '\n';
    // load preamble
    if (_preCheck(_config["preamble"], DType::STRING, false)) {
        std::string preamble_str = _asStr(_config["preamble"]);
        trim(preamble_str);
        std::cout << "Preamble Text:\n" << preamble_str << "\n";
        Alg alg(preamble_str);
        alg.write(_f(), _langStr());
    }
}

void Export::_generateChannels() {
    std::ifstream channel_file(appDir() + "/../include/mmcesim/copy/channel." + _langMmcesimExtension());
    std::string channel_content = "";
    if (!channel_file.is_open()) errorExit(Err::CANNOT_COPY_FROM_INCLUDE);
    while (!channel_file.eof()) {
        channel_content += channel_file.get();
    }
    channel_content.erase(channel_content.end() - 1); // last read character is invalid, erase it
    _f() << channel_content << '\n';
    std::ifstream functions_file(appDir() + "/../include/mmcesim/copy/functions." + _langMmcesimExtension());
    std::string functions_content = "";
    if (!functions_file.is_open()) errorExit(Err::CANNOT_COPY_FROM_INCLUDE);
    while (!functions_file.eof()) {
        functions_content += functions_file.get();
    }
    functions_content.erase(functions_content.end() - 1); // last read character is invalid, erase it
    _f() << functions_content << '\n';
    if (!_preCheck(_config["nodes"], DType::SEQ)) {
        std::cerr << "No channel node defined!\n";
        // TODO: error handling here
    }
    if (_config["channels"].size() != 1) {
        std::cerr << "So far, no RIS is allowed.\n";
        // TODO: for cascaded channel
    }
    bool gain_normal;
    std::string gain_param1, gain_param2;
    try {
        std::tie(gain_normal, gain_param1, gain_param2) = _setChannelGains(_config["channels"][0]);
    } catch (...) {
        // TODO: error handling
    }
    bool off_grid = true;
    if (_preCheck(_config["physics"]["off_grid"], DType::BOOL, false) &&
        !_config["physics"]["off_grid"].as<bool>()) {
        off_grid = false;
    }
    std::string sparsity = _asStr(_config["channels"][0]["sparsity"]);
    std::string channel_name = _asStr(_config["channels"][0]["id"]);
    if (lang == Lang::CPP) {
        _f() << "namespace mmce {\nbool generateChannels() {" << '\n';
        std::cout << "Tx index: " << _transmitters[0] << ", Rx index: " << _receivers[0] << '\n';
        auto&& t_node = _config["nodes"][_transmitters[0]];
        auto&& r_node = _config["nodes"][_receivers[0]];
        // size_t Mx, My, GMx, GMy, BMx, BMy, Nx, Ny, GNx, GNy, BNx, BNy;
        auto [Mx, My, GMx, GMy, BMx, BMy] = _getSize(r_node);
        auto [Nx, Ny, GNx, GNy, BNx, BNy] = _getSize(t_node);
        _f() << "std::filesystem::create_directory(\"_data\");\n"
             << "cx_mat " << _noise
             << " = arma::randn<mat>(" << BMx * BMy << "*" << BNx * BNy << ", " << _max_test_num
             << ") + 1i * arma::randn<mat>(" << BMx * BMy << "*" << BNx * BNy << ", " << _max_test_num << ");\n"
             << _noise << ".save(\"_data/" << _noise << ".bin\");\n"
             << "for (unsigned i = 0; i != " << _max_test_num << "; ++i) {\n"
             << "cx_mat " << channel_name << " = mmce::channel("
             << Mx << "," << My << ","
             << Nx << "," << Ny << ","
             << GMx << "," << GMy << ","
             << GNx << "," << GNy << ","
             << sparsity << ","
             << gain_normal << "," << gain_param1 << "," << gain_param2 << ","
             << off_grid
             << ");"
             << channel_name << ".save(\"_data/" << channel_name << "\" + std::to_string(i) + \".bin\");}";
        _f() << "}}\n\n";
    }
    // TODO: Generate channels.
}

void Export::_algorithms() {
    // search through all used algorithms,
    // and generate them here as a function.
}

void Export::_sounding() {
    auto&& t_node = _config["nodes"][_transmitters[0]];
    auto&& r_node = _config["nodes"][_receivers[0]];
    // size_t Mx, My, GMx, GMy, BMx, BMy, Nx, Ny, GNx, GNy, BNx, BNy;
    auto [Mx, My, GMx, GMy, BMx, BMy] = _getSize(r_node);
    auto [Nx, Ny, GNx, GNy, BNx, BNy] = _getSize(t_node);
    if (lang == Lang::CPP) {
        _f() << "int main(int argc, char* argv[]) {\n"
             << "arma_rng::set_seed_random();\n"
             << "mmce::generateChannels();\n"
             << "cx_mat " << _beamforming_W << " = "
             << "randn<mat>(" << Nx * Ny << ", " << BNx * BNy
             << ") + 1i * randn<mat>(" << Nx * Ny << ", " << BNx * BNy << ");\n"
             << _beamforming_W << " = normalise(" << _beamforming_W << ",2,0);\n"
             << "cx_mat " << _beamforming_F << " = "
             << "randn<mat>(" << Mx * My << ", " << BMx * BMy
             << ") + 1i * randn<mat>(" << Mx * My << ", " << BMx * BMy << ");\n"
             << _beamforming_F << " = normalise(" << _beamforming_F << ",2,0);\n\n"
             << "cx_mat " << _noise << ";\n"
             << "if (!" << _noise << ".load(\"_data/" << _noise << ".bin\", arma::arma_binary)) {\n"
             << "std::cerr << \"ERROR: Failed to load '" << _noise
             << ".bin' from '_data'.\" << std::endl; return 1;}\n";
        auto&& jobs = _config["simulation"]["jobs"];
        unsigned job_cnt = 0;
        for (auto&& job : jobs) {
            unsigned test_num = _getTestNum(job);
            auto&& SNR = job["SNR"];
            std::string SNR_mode;
            try {
                SNR_mode = boost::algorithm::to_lower_copy(_asStr(job["SNR_mode"], false));
            } catch (...) {
                SNR_mode = "db";
            }
            auto&& pilot = job["pilot"];
            Value_Vec<double> SNR_vec(SNR, true);
            Value_Vec<unsigned> pilot_vec(pilot, true);
            bool has_loop = true;
            if (SNR_vec.size() > 1) {
                _f() << "\ndouble NMSE" << job_cnt << "[" << SNR_vec.size() << "] = {};";
                if (SNR_mode == "linear") {
                    _f() << "vec SNR_linear = { " << SNR_vec.asStr() << " };\n";
                } else {
                    // default as dB
                    _f() << "vec SNR_dB = { " << SNR_vec.asStr() << " };\n"
                         << "vec SNR_linear = arma::exp10(SNR_dB / 10.0);\n";
                }
                _f() << "unsigned pilot = " << pilot_vec[0] << ";\n"
                     << "vec sigma2_all = arma::ones<vec>(SNR_dB.n_elem) / SNR_linear;\n"
                     << "for (uword ii = 0; ii != SNR_dB.n_elem; ++ii) {\n"
                     << "double sigma2 = sigma2_all[ii];\n";
            } else if (pilot_vec.size() > 1) {
                _f() << "\ndouble NMSE" << job_cnt << "[" << pilot_vec.size() << "] = {};";
                if (SNR_mode == "linear") {
                    _f() << "double SNR_linear = " << SNR_vec[0] << ";\n";
                } else {
                    // default as dB
                    _f() << "double SNR_dB = " << SNR_vec[0] << ";\n"
                         << "double SNR_linear = std::exp10(SNR_dB / 10.0);\n";
                }
                _f() << "double sigma2 = 1.0 / SNR_linear;\n"
                     << "uvec pilots = { " << pilot_vec.asStr() << " };\n"
                     << "for (uword ii = 0; ii != pilots.n_elem; ++ii) {\n"
                     << "double pilot = pilots[ii];\n";
            } else {
                has_loop = false;
                _f() << "\ndouble NMSE" << job_cnt << " = 0;";
            }
            _f() << "for (unsigned test_n = 0; test_n != " << test_num << "; ++test_n) {\n";
            for (auto&& channel : _config["channels"]) {
                // Load channel matrices.
                std::string ch = channel["id"].as<std::string>();
                _f() << "cx_mat " << ch << ";\n"
                     << "if (!" << ch << ".load(\"_data/" << ch
                     << "\" + std::to_string(test_n) + \".bin\", arma::arma_binary)) {\n"
                     << "std::cerr << \"ERROR: Failed to load '" << ch << "\" + std::to_string(test_n) + \""
                     << ".bin' from '_data'.\" << std::endl; return 1;}\n";
            }
            // TODO: The cascaded channel is the only channel is valid only for a simple MIMO system.
            _f() << "cx_mat " << _cascaded_channel << " = " << _config["channels"][0]["id"].as<std::string>()
                 << ";\ncx_vec " << _received_signal << " = arma::kron("
                 << _beamforming_F << ".st()," << _beamforming_W << ".t())"
                 << "*" << _cascaded_channel << ".as_col();\n"
                 << "cx_vec this_noise = " << _noise << ".col(test_n);\n"
                 << "double noise_power = arma::accu(arma::pow(arma::abs(this_noise), 2));\n"
                 << "double raw_signal_power = arma::accu(arma::pow(arma::abs("
                 << _received_signal << "), 2));\n"
                 << _received_signal << " += std::sqrt(raw_signal_power / noise_power * sigma2) * this_noise;\n";
            _estimation();
            _f() << "}\n";
            if (has_loop) _f() << "}\n";
            ++job_cnt;
        }
    }
}

void Export::_estimation() {
    Macro macro;
    auto&& jobs = _config["simulation"]["jobs"];
    if (_preCheck(jobs, DType::SEQ)) {
        macro.job_num = jobs.size();
        for (size_t i = 0; i != macro.job_num; ++i) {
            auto&& job_algs = jobs[i]["algorithms"];
            macro.alg_num.push_back(job_algs.size());
            std::vector<std::string> alg_names;
            std::vector<std::string> alg_params;
            for (auto&& alg : job_algs) {
                alg_names.push_back(_asStr(alg["alg"]));
                // TODO: macro parameters
                alg_params.push_back("");
            }
            macro.alg_names.push_back(alg_names);
            macro.alg_params.push_back(alg_params);
        }
    }
    std::string estimation_str;
    if (!_preCheck(_config["estimation"], DType::STRING)) {
        // maybe add a message on the console
        estimation_str = "auto";
    } else {
        estimation_str = _asStr(_config["estimation"]);
    }
    trim(estimation_str);
    std::cout << estimation_str << "\n";
    if (estimation_str.length() == 4 && boost::algorithm::to_lower_copy(estimation_str) == "auto") {
        // TODO: use tasked-oriented algorithm
        // just do nothing right now
        std::cout << "Use auto estimation." << std::endl;
    } else {
        Alg alg(estimation_str, macro);
        alg.write(_f(), _langStr());
    }
}

void Export::_reporting() {

}

void Export::_ending() {
    if (lang == Lang::CPP) {
        _f() << "return 0;\n}\n";
    }
}

bool Export::_setTransmitterReceiver() {
    if (!_preCheck(_config["nodes"], DType::SEQ)) return false;
    auto&& nodes = _config["nodes"];
    for (int i = 0; i != nodes.size(); ++i) {
        auto node = nodes[i]["role"];
        if (!_preCheck(node, DType::STRING | DType::UNDEF)) return false;
        if (node.IsDefined()) {
            if (auto s = boost::algorithm::to_lower_copy(node.as<std::string>());
                s == "transmitter" || s == "transmit" || s == "tx" || s == "t") {
                _transmitters.push_back(i);
            } else if (s == "receiver" || s == "receive" || s == "rx" || s == "r") {
                _receivers.push_back(i);
            }
        }
    }
    if (_transmitters.size() > _MAX_TX) {
        YAML_Error e("Too many transmitters. In mmCEsim " + _MMCESIM_VER_STR +
            " there can be at most " + std::to_string(_MAX_TX) + " transmitters.", Err::TOO_MANY_TX);
        return false;
    }
    if (_receivers.size() > _MAX_RX) {
        YAML_Error e("Too many receivers. In mmCEsim " + _MMCESIM_VER_STR +
            " there can be at most " + std::to_string(_MAX_RX) + " receivers.", Err::TOO_MANY_RX);
        return false;
    }
    return true;
}

bool Export::_setMaxTestNum() {
    if (!_preCheck(_config["simulation"]["jobs"], DType::SEQ)) return false;
    auto&& jobs = _config["simulation"]["jobs"];
    for (auto&& job : jobs) {
        if (_preCheck(job["test_num"], DType::INT, false)) {
            unsigned test_num = job["test_num"].as<unsigned>();
            if (test_num > _max_test_num) _max_test_num = test_num;
        } else {
            if (500 > _max_test_num) _max_test_num = 500;
        }
    }
    if (_max_test_num == 0) _max_test_num = 500; // default as 500.
    return true;
}

bool Export::_setVarNames() {
    try {
        _cascaded_channel = _asStr(_config["sounding"]["variables"]["channel"]);
    } catch (...) {
        _cascaded_channel = "H_cascaded";
    }
    try {
        _received_signal = _asStr(_config["sounding"]["variables"]["received"]);
    } catch (...) {
        _received_signal = "y";
    }
    try {
        _noise = _asStr(_config["sounding"]["variables"]["noise"]);
    } catch (...) {
        _noise = "n";
    }
    try {
        _beamforming_F = _asStr(_config["nodes"][_transmitters[0]]["beamforming"]["variable"]);
    } catch (...) {
        _beamforming_F = "F";
    }
    try {
        _beamforming_W = _asStr(_config["nodes"][_receivers[0]]["beamforming"]["variable"]);
    } catch (...) {
        _beamforming_W = "W";
    }
    return true;
}

std::tuple<unsigned, unsigned, unsigned, unsigned, unsigned, unsigned> Export::_getSize(const YAML::Node& n) {
    auto&& size_n = n["size"];
    Value_Vec<size_t> value_vec(size_n, false, 1);
    size_t Mx = value_vec[0];
    size_t My = value_vec[1];
    size_t GMx;
    size_t GMy = 1;
    size_t BMx;
    size_t BMy = 1;
    auto&& grid_n = n["grid"];
    try {
        std::string GM_str = grid_n.as<std::string>();
        if (boost::algorithm::to_lower_copy(GM_str) == "same") {
            GMx = Mx;
            GMy = My;
        } else {
            Value_Vec<size_t> GM_value_vec(grid_n, false, 1);
            GMx = GM_value_vec[0];
            GMy = GM_value_vec[1];
        }
    } catch (...) {
        Value_Vec<size_t> GM_value_vec(grid_n, false, 1);
        GMx = GM_value_vec[0];
        GMy = GM_value_vec[1];
    }
    auto&& beam_n = n["beam"];
    try {
        std::string BM_str = beam_n.as<std::string>();
        if (boost::algorithm::to_lower_copy(BM_str) == "same") {
            BMx = Mx;
            BMy = My;
        } else {
            Value_Vec<size_t> BM_value_vec(beam_n, false, 1);
            BMx = BM_value_vec[0];
            BMy = BM_value_vec[1];
        }
    } catch (...) {
        Value_Vec<size_t> BM_value_vec(beam_n, false, 1);
        BMx = BM_value_vec[0];
        BMy = BM_value_vec[1];
    }
    return { Mx, My, GMx, GMy, BMx, BMy };
}
