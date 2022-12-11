/**
 * @file export.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Export Class
 * @version 0.1.0
 * @date 2022-10-17
 *
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "export.h"

Export::Export(CLI_Options& opt, Shared_Info* const info) : _opt(opt), _s_info(info) {
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

Export::Export(CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors, Shared_Info* const info)
    : _opt(opt), _config(config), _errors(errors), _s_info(info) {
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
    _setCascadedChannel();
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

YAML_Errors Export::exportCode(CLI_Options& opt, Shared_Info* const info) {
    Export ep(opt, info);
    return ep.exportCode();
}

YAML_Errors Export::exportCode(CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors,
                               Shared_Info* const info) {
    Export ep(opt, config, errors, info);
    return ep.exportCode();
}

bool Export::_preCheck(const YAML::Node& n, unsigned a_t, bool mattered) {
    bool find_match = false;
    if (a_t & DType::INT && n.IsScalar()) {
        find_match = true;
        try {
            n.as<int>();
        } catch (...) { find_match = false; }
    }
    if (!find_match && a_t & DType::DOUBLE && n.IsScalar()) {
        find_match = true;
        try {
            n.as<double>();
        } catch (...) { find_match = false; }
    }
    if (!find_match && a_t & DType::STRING && n.IsScalar()) {
        find_match = true;
        try {
            n.as<std::string>();
        } catch (...) { find_match = false; }
    }
    if (!find_match && a_t & DType::BOOL && n.IsScalar()) {
        find_match = true;
        try {
            n.as<bool>();
        } catch (...) { find_match = false; }
    }
    if (!find_match && a_t & DType::CHAR && n.IsScalar()) {
        find_match = true;
        try {
            n.as<char>();
        } catch (...) { find_match = false; }
    }
    if (!find_match && a_t & DType::SEQ && n.IsSequence()) { find_match = true; }
    if (!find_match && a_t & DType::MAP && n.IsMap()) { find_match = true; }
    if (!find_match && a_t & DType::NUL && n.IsNull()) { find_match = true; }
    if (!find_match && a_t & DType::UNDEF && !n.IsDefined()) { find_match = true; }
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
    _info(std::string("This is mmCEsim ") + _MMCESIM_VER_STR + ", with target version " +
          _config["_compiled"]["version_str"].as<std::string>() + ".");
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
            } catch (...) {
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
    std::cout << "Set Channel Gains!\n";
    try {
        if (auto&& gains = n["gains"]; _preCheck(gains, DType::MAP, false)) {
            if (auto mode = boost::algorithm::to_lower_copy(_asStr(gains["mode"])); mode == "normal") {
                std::string mean     = _asStr(gains["mean"]);
                std::string variance = _asStr(gains["variance"]);
                return {true, mean, variance};
            } else if (mode == "uniform") {
                std::string min = _asStr(gains["min"]);
                std::string max = _asStr(gains["max"]);
                return {false, min, max};
            }
        } else if (_preCheck(n["node"], DType::UNDEF)) {
            // default value if not specified
            return {true, "0", "1"};
        } else {
            return {true, "0", "1"};
            // throw("Channel gains not correctly set!");
        }
    } catch (...) {
        // do nothing
    }
    return {true, "0", "1"};
}

void Export::_topComment() {
    // TODO: ipynb settings
    std::string title;
    try {
        title = _asStr(_config["meta"]["title"], false);
        if (title == "") throw("Title empty!");
    } catch (...) {
        title = _opt.output; // TODO: Only file name.
    }
    _wComment() << "Title: " << title << '\n';
    try {
        std::string desc = _asStr(_config["meta"]["description"], false);
        _wComment() << "Description: " << desc << '\n';
    } catch (...) {}
    try {
        std::string author = _asStr(_config["meta"]["author"], false);
        _wComment() << "Author: " << author << '\n';
    } catch (...) {}

    // get the current time
    std::time_t curr_time   = std::time(nullptr);
    std::tm curr_tm         = *std::localtime(&curr_time);
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
        _wComment() << "$ g++ " << _opt.output << " -std=c++17 -larmadillo -O3\n";
        _wComment() << "or\n";
        _wComment() << "$ clang++ " << _opt.output << " -std=c++17 -larmadillo -O3\n";
        _wComment() << "or just link to Armadillo library with whatever compiler you have.\n";
        // set cpp compile command
        if (_s_info) {
            _s_info->backend         = "cpp";
            _s_info->src_compile_cmd = fmt::format("{{}} {} -std=c++17 -larmadillo -O3 {{}}", _opt.output);
        }
    }
    _f() << "\n";
}

void Export::_beginning() {
    // load header
    std::ifstream header_file(appDir() + "/../include/mmcesim/copy/header." + _langMmcesimExtension());
    std::string header_content = "";
    while (!header_file.eof()) { header_content += header_file.get(); }
    header_content.erase(header_content.end() - 1); // last read character is invalid, erase it
    _f() << header_content << '\n';
}

void Export::_generateChannels() {
    std::ifstream channel_file(appDir() + "/../include/mmcesim/copy/channel." + _langMmcesimExtension());
    std::string channel_content = "";
    if (!channel_file.is_open()) errorExit(Err::CANNOT_COPY_FROM_INCLUDE);
    while (!channel_file.eof()) { channel_content += channel_file.get(); }
    channel_content.erase(channel_content.end() - 1); // last read character is invalid, erase it
    _f() << channel_content << '\n';
    std::ifstream functions_file(appDir() + "/../include/mmcesim/copy/functions." + _langMmcesimExtension());
    std::string functions_content = "";
    if (!functions_file.is_open()) errorExit(Err::CANNOT_COPY_FROM_INCLUDE);
    while (!functions_file.eof()) { functions_content += functions_file.get(); }
    functions_content.erase(functions_content.end() - 1); // last read character is invalid, erase it
    _f() << functions_content << '\n';
    if (!_preCheck(_config["nodes"], DType::SEQ)) {
        std::cerr << "No channel node defined!\n";
        // TODO: error handling here
    }
    bool off_grid = true;
    if (_preCheck(_config["physics"]["off_grid"], DType::BOOL, false) && !_config["physics"]["off_grid"].as<bool>()) {
        off_grid = false;
    }
    std::string freq  = "narrow";
    unsigned carriers = 1;
    if (auto&& n = _config["physics"]["frequency"]; _preCheck(n, DType::STRING, false)) { freq = _asStr(n); }
    if (auto&& m = _config["physics"]["carriers"]; _preCheck(m, DType::INT, false)) { carriers = m.as<unsigned>(); }
    auto&& t_node                     = _config["nodes"][_transmitters[0]];
    auto&& r_node                     = _config["nodes"][_receivers[0]];
    auto [Mx, My, GMx, GMy, BMx, BMy] = _getSize(r_node);
    auto [Nx, Ny, GNx, GNy, BNx, BNy] = _getSize(t_node);
    if (lang == Lang::CPP) {
        _f() << "namespace mmce {\nbool generateChannels() {" << '\n';
        std::cout << "Tx index: " << _transmitters[0] << ", Rx index: " << _receivers[0] << '\n';
        _f() << "std::filesystem::create_directory(\"_data\");\n"
             << (freq == "wide" ? "cx_cube " : "cx_mat ") << _noise;
        if (freq == "wide") {
            _f() << " = arma::randn<cube>(" << BMx * BMy << "*" << BNx * BNy << ", " << carriers << ", "
                 << _max_test_num << ") + 1i * arma::randn<cube>(" << BMx * BMy << "*" << BNx * BNy << ", " << carriers
                 << ", " << _max_test_num << ");\n";
        } else {
            _f() << " = arma::randn<mat>(" << BMx * BMy << "*" << BNx * BNy << ", " << _max_test_num
                 << ") + 1i * arma::randn<mat>(" << BMx * BMy << "*" << BNx * BNy << ", " << _max_test_num << ");\n";
        }
        _f() << _noise << ".save(\"_data/" << _noise << ".bin\");\n";
        _f() << "for (unsigned i = 0; i != " << _max_test_num << "; ++i) {\n";
    }
    for (unsigned i = 0; i != _config["channels"].size(); ++i) {
        auto&& ch = _config["channels"][i];
        std::cout << "from: " << _channel_graph.from[i] << ", to: " << _channel_graph.to[i] << "\n";
        if (_channel_graph.from[i] > _channel_graph.nodes.size()) {
            std::string err =
                fmt::format("Unknown 'from' node '{}' in channel '{}'!", _asStr(ch["from"]), _asStr(ch["id"]));
            std::cerr << "[mmcesim] export $ ERROR: " << err << '\n';
            throw(err);
        }
        if (_channel_graph.to[i] > _channel_graph.nodes.size()) {
            std::string err =
                fmt::format("Unknown 'to' node '{}' in channel '{}'!", _asStr(ch["from"]), _asStr(ch["id"]));
            std::cerr << "[mmcesim] export $ ERROR: " << err << '\n';
            throw(err);
        }
        auto&& t_node                     = _config["nodes"][_channel_graph.from[i]];
        auto&& r_node                     = _config["nodes"][_channel_graph.to[i]];
        auto [Mx, My, GMx, GMy, BMx, BMy] = _getSize(r_node);
        auto [Nx, Ny, GNx, GNy, BNx, BNy] = _getSize(t_node);
        bool gain_normal;
        std::string gain_param1, gain_param2;
        try {
            std::tie(gain_normal, gain_param1, gain_param2) = _setChannelGains(ch);
        } catch (const std::exception& e) {
            // TODO: error handling
            std::cout << "Setting Channel Gains Error!\n" << e.what() << "\n";
        }
        std::string sparsity     = _asStr(ch["sparsity"]);
        std::string channel_name = _asStr(ch["id"]);
        if (lang == Lang::CPP) {
            if (freq == "wide") _f() << "cx_cube " << channel_name << " = mmce::wide_channel(" << carriers << ",";
            else _f() << "cx_mat " << channel_name << " = mmce::channel(";
            _f() << Mx << "," << My << "," << Nx << "," << Ny << "," << GMx << "," << GMy << "," << GNx << "," << GNy
                 << "," << sparsity << "," << gain_normal << "," << gain_param1 << "," << gain_param2 << "," << off_grid
                 << ");" << channel_name << ".save(\"_data/" << channel_name << "\" + std::to_string(i) + \".bin\");";
        }
    }
    if (lang == Lang::CPP) { _f() << "}return true;}}\n\n"; }
    // TODO: Generate channels.
}

void Export::_algorithms() {
    _loadALG();
    // prepare macros
    auto&& t_node                     = _config["nodes"][_transmitters[0]];
    auto&& r_node                     = _config["nodes"][_receivers[0]];
    auto [Mx, My, GMx, GMy, BMx, BMy] = _getSize(r_node);
    auto [Nx, Ny, GNx, GNy, BNx, BNy] = _getSize(t_node);
    std::string freq                  = "narrow";
    unsigned carriers                 = 1;
    if (auto&& n = _config["physics"]["frequency"]; _preCheck(n, DType::STRING, false)) { freq = _asStr(n); }
    if (auto&& m = _config["physics"]["carriers"]; _preCheck(m, DType::INT, false)) { carriers = m.as<unsigned>(); }
    auto&& jobs = _config["simulation"]["jobs"];
    Macro macro;
    macro._cascaded_channel         = _cascaded_channel;
    macro.job_num                   = jobs.size();
    macro._N                        = {Nx, Ny, Mx, My};
    macro._B                        = {BNx, BNy, BMx, BMy};
    macro._G                        = {GNx, GNy, GMx, GMy};
    auto&& common_custom_macro_node = _config["macro"];
    if (_preCheck(common_custom_macro_node, DType::SEQ, false)) {
        for (auto&& macro_pair : common_custom_macro_node) {
            bool in_alg        = false;
            auto&& in_alg_node = macro_pair["in_alg"];
            if (_preCheck(in_alg_node, DType::BOOL, false)) { in_alg = in_alg_node.as<bool>(); }
            if (!in_alg) { macro.custom.push_back({_asStr(macro_pair["name"]), _asStr(macro_pair["value"])}); }
        }
    }
    // load preamble
    if (_preCheck(_config["preamble"], DType::STRING, false)) {
        std::string preamble_str = _asStr(_config["preamble"]);
        trim(preamble_str);
        std::cout << "Preamble Text:\n" << preamble_str << "\n";
        Alg alg(preamble_str, macro);
        alg.write(_f(), _langStr());
    }
}

void Export::_sounding() {
    auto&& t_node                     = _config["nodes"][_transmitters[0]];
    auto&& r_node                     = _config["nodes"][_receivers[0]];
    auto [Mx, My, GMx, GMy, BMx, BMy] = _getSize(r_node);
    auto [Nx, Ny, GNx, GNy, BNx, BNy] = _getSize(t_node);
    std::string freq                  = "narrow";
    unsigned carriers                 = 1;
    if (auto&& n = _config["physics"]["frequency"]; _preCheck(n, DType::STRING, false)) { freq = _asStr(n); }
    if (auto&& m = _config["physics"]["carriers"]; _preCheck(m, DType::INT, false)) { carriers = m.as<unsigned>(); }
    if (lang == Lang::CPP) {
        _f() << "int main(int argc, char* argv[]) {\n"
             << "arma_rng::set_seed_random();\n"
             << "mmce::generateChannels();\n"
             << (freq == "wide" ? "cx_cube " : "cx_mat ") << _noise << ";\n"
             << "if (!" << _noise << ".load(\"_data/" << _noise << ".bin\", arma::arma_binary)) {\n"
             << "std::cerr << \"ERROR: Failed to load '" << _noise
             << ".bin' from '_data'.\" << std::endl; return 1;}\n";
        if (freq == "wide") { _f() << "uword carriers_num = " << carriers << ";\n"; }
        auto&& jobs      = _config["simulation"]["jobs"];
        unsigned job_cnt = 0;
        for (auto&& job : jobs) {
            unsigned test_num = _getTestNum(job);
            auto&& SNR        = job["SNR"];
            std::string SNR_mode;
            try {
                SNR_mode = boost::algorithm::to_lower_copy(_asStr(job["SNR_mode"], false));
            } catch (...) { SNR_mode = "db"; }
            auto&& pilot = job["pilot"];
            Value_Vec<double> SNR_vec(SNR, true);
            Value_Vec<unsigned> pilot_vec(pilot, true);
            bool has_loop = true;
            if (SNR_vec.size() > 1) {
                _f() << "\nmat NMSE" << job_cnt << " = arma::zeros(" << SNR_vec.size() << ", "
                     << job["algorithms"].size() << ");"
                     << "{"; //  Start a group
                if (SNR_mode == "linear") {
                    _f() << "vec SNR_linear = { " << SNR_vec.asStr() << " };\n"
                         << "vec SNR_dB = 10 * arma::log10(SNR_linear / 10.0);\n";
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
                _f() << "\nmat NMSE" << job_cnt << " = arma::zeros(" << pilot_vec.size() << ", "
                     << job["algorithms"].size() << ");"
                     << "{"; //  Start a group
                if (SNR_mode == "linear") {
                    _f() << "double SNR_linear = " << SNR_vec[0] << ";\n";
                } else {
                    // default as dB
                    _f() << "double SNR_dB = " << SNR_vec[0] << ";\n"
                         << "double SNR_linear = std::pow(10.0, SNR_dB / 10.0);\n";
                }
                _f() << "double sigma2 = 1.0 / SNR_linear;\n"
                     << "uvec pilots = { " << pilot_vec.asStr() << " };\n"
                     << "for (uword ii = 0; ii != pilots.n_elem; ++ii) {\n"
                     << "double pilot = pilots[ii];\n";
            } else {
                has_loop = false;
                _f() << "\nmat NMSE" << job_cnt << " = arma::zeros(1, " << job["algorithms"].size() << ");"
                     << "{"; // Start a group
                _f() << "double SNR_dB = " << SNR_vec[0] << ";\n"
                     << "double SNR_linear = std::pow(10.0, SNR_dB / 10.0);\n"
                     << "double sigma2 = 1.0 / SNR_linear;\n";
                _f() << "unsigned pilot = " << pilot_vec[0] << ";\n";
                _f() << "uword ii = 0;\n";
            }
            _f() << "cx_cube " << _beamforming_W << " = "
                 << "randn<cube>(" << Nx * Ny << ", " << BNx * BNy << ", pilot / " << BNx * BNy
                 << ") + 1i * randn<cube>(" << Nx * Ny << ", " << BNx * BNy << ", pilot / " << BNx * BNy << ");\n"
                 << _beamforming_W << ".each_slice([](cx_mat& X){return normalise(X,2,0);});\n"
                 << "cx_cube " << _beamforming_F << " = "
                 << "randn<cube>(" << Mx * My << ", " << BMx * BMy << ", pilot / " << BNx * BNy
                 << ") + 1i * randn<cube>(" << Mx * My << ", " << BMx * BMy << ", pilot / " << BNx * BNy << ");\n"
                 << _beamforming_F << ".each_slice([](cx_mat& X){return normalise(X,2,0);});\n\n";
            _f() << "for (unsigned test_n = 0; test_n != " << test_num << "; ++test_n) {\n";
            for (auto&& channel : _config["channels"]) {
                // Load channel matrices.
                std::string ch = channel["id"].as<std::string>();
                _f() << (freq == "wide" ? "cx_cube " : "cx_mat ") << ch << ";\n"
                     << "if (!" << ch << ".load(\"_data/" << ch
                     << "\" + std::to_string(test_n) + \".bin\", arma::arma_binary)) {\n"
                     << "std::cerr << \"ERROR: Failed to load '" << ch << "\" + std::to_string(test_n) + \""
                     << ".bin' from '_data'.\" << std::endl; return 1;}\n";
            }
            // TODO: The cascaded channel is the only channel is valid only for a simple MIMO system.
            // TODO: transmit pilots!
            if (freq == "wide") {
                _f() << "cx_mat " << _received_signal << "(pilot*" << BMx * BMy << ", carriers_num);"
                     << "cx_cube " << _cascaded_channel << "(" << Mx * My << ", " << Nx * Ny
                     << ", carriers_num, arma::fill::zeros);\n"
                     << "for (unsigned k = 0; k != carriers_num; ++k) {\n";
                if (!_channel_graph.paths.empty())
                    for (unsigned i = 1; i < _channel_graph.paths[0].size(); ++i) {
                        // TODO: generate channel
                    }
                _f() << "}\nfor (uword t = 0; t < pilot / " << BNx * BNy << "; ++t) {\n"
                     << "const cx_mat& _F = " << _beamforming_F << ".slice(t);"
                     << "const cx_mat& _W = " << _beamforming_W << ".slice(t);\n"
                     << "for (uword k = 0; k != carriers_num; ++k) {"
                     << "cx_vec _y = arma::kron(_F.st(), _W.t()) * " << _cascaded_channel << ".slice(k).as_col();\n"
                     << "cx_vec this_noise = " << _noise << ".slice(test_n).col(k);\n"
                     << "double noise_power = arma::accu(arma::pow(arma::abs(this_noise), 2));\n"
                     << "double raw_signal_power = arma::accu(arma::pow(arma::abs(_y), 2));\n"
                     << "_y += std::sqrt(raw_signal_power / noise_power * sigma2) * this_noise;\n"
                     << _received_signal << "(arma::span(t * " << BNx * BNy * BMx * BMy << ",(t+1)*"
                     << BNx * BNy * BMx * BMy << "-1), k) = _y;}}\n";
            } else {
                _f() << "cx_vec " << _received_signal << "(pilot*" << BMx * BMy << ");"
                     << "cx_mat " << _cascaded_channel << " = " << _config["channels"][0]["id"].as<std::string>() << ";"
                     << "for (uword t = 0; t < pilot / " << BNx * BNy << "; ++t) {\n"
                     << "const cx_mat& _F = " << _beamforming_F << ".slice(t);"
                     << "const cx_mat& _W = " << _beamforming_W << ".slice(t);\n"
                     << "cx_vec _y = arma::kron(_F.st(), _W.t()) * " << _cascaded_channel << ".as_col();\n"
                     << "cx_vec this_noise = " << _noise << ".col(test_n);\n"
                     << "double noise_power = arma::accu(arma::pow(arma::abs(this_noise), 2));\n"
                     << "double raw_signal_power = arma::accu(arma::pow(arma::abs(_y), 2));\n"
                     << "_y += std::sqrt(raw_signal_power / noise_power * sigma2) * this_noise;\n"
                     << _received_signal << "(arma::span(t * " << BNx * BNy * BMx * BMy << ",(t+1)*"
                     << BNx * BNy * BMx * BMy << "-1)) = _y;}\n";
            }
            Macro macro;
            macro._cascaded_channel = _cascaded_channel;
            macro.job_num           = jobs.size();
            macro._N                = {Nx, Ny, Mx, My};
            macro._B                = {BNx, BNy, BMx, BMy};
            macro._G                = {GNx, GNy, GMx, GMy};
            for (size_t i = 0; i != macro.job_num; ++i) {
                auto&& job_algs = jobs[i]["algorithms"];
                macro.alg_num.push_back(job_algs.size());
                std::vector<std::string> alg_names;
                std::vector<std::string> alg_params;
                std::vector<std::vector<std::pair<std::string, std::string>>> alg_custom;
                for (auto&& alg : job_algs) {
                    auto alg_name = _asStr(alg["alg"]);
                    alg_names.push_back(alg_name);
                    // TODO: macro parameters
                    if (alg_name == "OMP") {
                        if (_preCheck(alg["max_iter"], DType::INT, false)) {
                            alg_params.push_back(_asStr(alg["max_iter"]));
                        } else if (_preCheck(alg["sparsity"], DType::INT, false)) {
                            alg_params.push_back(_asStr(alg["sparsity"]));
                        } else {
                            // TODO: the default iteration of OMP
                            alg_params.push_back("100");
                        }
                    } else {
                        // for custom functions
                        auto&& params_node = alg["params"];
                        if (params_node.IsDefined()) {
                            if (params_node.IsSequence()) {
                                std::string params;
                                for (size_t j = 0; j + 1 != params_node.size(); ++j) {
                                    params += params_node[j].as<std::string>() + " ";
                                }
                                params += params_node[params_node.size() - 1].as<std::string>();
                                alg_params.push_back(params);
                            } else {
                                alg_params.push_back(params_node.as<std::string>());
                            }
                        } else alg_params.push_back("");
                    }
                    // add custom macros
                    std::vector<std::pair<std::string, std::string>> alg_custom_;
                    auto&& custom_macro_node = alg["macro"];
                    if (_preCheck(custom_macro_node, DType::SEQ, false)) {
                        for (auto&& macro_pair : custom_macro_node) {
                            alg_custom_.push_back({_asStr(macro_pair["name"]), _asStr(macro_pair["value"])});
                        }
                    }
                    alg_custom.push_back(alg_custom_);
                }
                auto&& common_custom_macro_node = _config["macro"];
                if (_preCheck(common_custom_macro_node, DType::SEQ, false)) {
                    for (auto&& macro_pair : common_custom_macro_node) {
                        bool in_alg        = false;
                        auto&& in_alg_node = macro_pair["in_alg"];
                        if (_preCheck(in_alg_node, DType::BOOL, false)) { in_alg = in_alg_node.as<bool>(); }
                        if (in_alg) {
                            macro.custom_in_alg.push_back({_asStr(macro_pair["name"]), _asStr(macro_pair["value"])});
                        } else {
                            macro.custom.push_back({_asStr(macro_pair["name"]), _asStr(macro_pair["value"])});
                        }
                    }
                }
                macro.alg_names.push_back(alg_names);
                macro.alg_params.push_back(alg_params);
                macro.alg_custom.push_back(alg_custom);
            }
            _estimation(macro, job_cnt);
            _f() << "}\n";
            if (has_loop) _f() << "}\n";
            _f() << "NMSE" << job_cnt << "/=" << test_num << ";";
            if (_preCheck(_config["conclusion"], DType::STRING, false)) {
                Alg a(_asStr(_config["conclusion"]), macro, job_cnt, -1);
                a.write(_f(), _langStr());
            }
            _f() << "}\n"; // End the job group.
            ++job_cnt;
        }
    }
}

void Export::_estimation(const Macro& macro, int job_cnt) {
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
        Alg alg(estimation_str, macro, job_cnt);
        alg.write(_f(), _langStr());
    }
}

void Export::_reporting() {
    auto&& report_format = _config["simulation"]["report"]["form"];
    // TODO: check report format field to decide report type
    auto&& report_name      = _config["simulation"]["report"]["name"];
    std::string report_file = "report.rpt";
    std::string tex_file    = "_tex_report/report.tex";
    if (_preCheck(report_name, DType::STRING, false)) {
        report_file = _asStr(report_name) + ".rpt";
        tex_file    = "_tex_report/" + _asStr(report_name) + ".tex";
    }
    _f() << "std::filesystem::create_directory(\"_tex_report\");\n";
    _f() << "std::ofstream report_file(\"" << report_file << "\");"
         << "if (report_file.is_open()) {";
    _f() << "std::ofstream tex_file(\"" << tex_file << "\");";
    auto&& jobs           = _config["simulation"]["jobs"];
    std::string sim_title = "mmCEsim Simulation Report";
    if (_preCheck(_config["meta"]["title"], DType::STRING, false)) { sim_title = _asStr(_config["meta"]["title"]); }
    std::string sim_description = "";
    if (_preCheck(_config["meta"]["description"], DType::STRING, false)) {
        sim_description = _asStr(_config["meta"]["description"]);
    }
    std::string sim_author = "";
    if (_preCheck(_config["meta"]["author"], DType::STRING, false)) { sim_author = _asStr(_config["meta"]["author"]); }
    std::string freq  = "narrow";
    unsigned carriers = 1;
    if (auto&& n = _config["physics"]["frequency"]; _preCheck(n, DType::STRING, false)) { freq = _asStr(n); }
    if (auto&& m = _config["physics"]["carriers"]; _preCheck(m, DType::INT, false)) { carriers = m.as<unsigned>(); }

    std::time_t curr_time   = std::time(nullptr);
    std::tm curr_tm         = *std::localtime(&curr_time);
    const char* time_format = "%F %T (UTC %z)";

    auto&& t_node                     = _config["nodes"][_transmitters[0]];
    auto&& r_node                     = _config["nodes"][_receivers[0]];
    auto [Mx, My, GMx, GMy, BMx, BMy] = _getSize(r_node);
    auto [Nx, Ny, GNx, GNy, BNx, BNy] = _getSize(t_node);

    std::string off_grid = "true";
    auto&& off_grid_node = _config["physics"]["off_grid"];
    if (_preCheck(off_grid_node, DType::BOOL, false)) {
        if (off_grid_node.as<bool>() == false) off_grid = "false";
    }

    std::string channel_sparsity;
    auto&& channel_sparsity_node = _config["channels"][0]["sparsity"];
    if (_preCheck(channel_sparsity_node, DType::INT, false)) {
        channel_sparsity = channel_sparsity_node.as<std::string>();
    }

    std::filesystem::path input_path(_opt.input);
    std::string sim_file = input_path.filename().replace_extension().string();
    std::string out_dir  = input_path.parent_path().string();
    if (out_dir == "") out_dir = ".";

    std::filesystem::create_directory(out_dir + "/_tex_report");
    std::filesystem::copy_file(appDir() + "/../include/mmcesim/tex/simreport.cls",
                               out_dir + "/_tex_report/simreport.cls",
                               std::filesystem::copy_options::overwrite_existing);
    std::filesystem::create_directory(out_dir + "/_tex_report/fig");
    std::filesystem::copy_file(appDir() + "/../include/mmcesim/tex/fig/mmCEsim_logo_256.png",
                               out_dir + "/_tex_report/fig/mmCEsim_logo_256.png",
                               std::filesystem::copy_options::overwrite_existing);

    _f() << "tex_file << \"\\\\documentclass[mmcesim]{simreport}\\n\";"
         << "tex_file << \"\\\\begin{document}\\n\";"
         << "tex_file << \"\\\\title{" << sim_title << "}\\n\";"
         << "tex_file << \"\\\\author{" << sim_author << "}\\n\";"
         << "tex_file << \"\\\\date{" << std::put_time(&curr_tm, "%F") << "}\\n\";"
         << "tex_file << \"\\\\rtime{" << std::put_time(&curr_tm, "%T") << "}\\n\";"
         << "tex_file << \"\\\\maketitle\\n"
         << sim_description << "\\n\";";
    _f() << "report_file << \"#" << std::string(78, '-') << "\\n\";"
         << "report_file << \"# Title      : " << sim_title << "\\n\";"
         << "report_file << \"# Description: " << sim_description << "\\n\";"
         << "report_file << \"# Author     : " << sim_author << "\\n\";"
         << "report_file << \"# Time       : " << std::put_time(&curr_tm, time_format) << "\\n\";"
         << "report_file << \"# \\n\";"
         << "report_file << \"# Report generated by " << _MMCESIM_NAME << ' ' << _MMCESIM_VER_STR << ".\\n\";"
         << "report_file << \"# GitHub organization at " << _MMCESIM_GIT << ".\\n\";"
         << "report_file << \"# Web app is available at " << _MMCESIM_WEBAPP << ".\\n\";"
         << "report_file << \"# Visit " << _MMCESIM_WEB << " for more information.\\n\";"
         << "report_file << \"#" << std::string(78, '-') << "\\n\\n\";"
         << "report_file << \"# System Settings\\n\\n\";"
         << "report_file << \"  Transmitter: " << Nx << "x" << Ny << ", Grid: " << GNx << "x" << GNy
         << ", Beam: " << BNx << "x" << BNy << "\\n\";"
         << "report_file << \"  Receiver: " << Mx << "x" << My << ", Grid: " << GMx << "x" << GMy << ", Beam: " << BMx
         << "x" << BMy << "\\n\";"
         << "report_file << \"  Channel Sparsity: " << channel_sparsity << "\\n\";"
         << "report_file << \"  Off Grid: " << off_grid << "\\n\";"
         << "report_file << \"  Bandwidth: " << (freq != "narrow" ? "Wide" : "Narrow") << "band\\n\";";
    if (freq != "narrow") _f() << "report_file << \"  Carriers: " << carriers << "\\n\";";
    _f() << "report_file << \"\\n\";"
         << "tex_file << \"\\\\simsystem{" << Nx << "}{" << Ny << "}{" << BNx << "}{" << BNy << "}{" << GNx << "}{"
         << GNy << "}{" << Mx << "}{" << My << "}{" << BMx << "}{" << BMy << "}{" << GMx << "}{" << GMy << "}{"
         << channel_sparsity << "}{" << off_grid << "}{" << (freq != "narrow" ? "1" : "0") << "}{" << carriers
         << "}\\n\";";
    for (unsigned job_cnt = 0; job_cnt != jobs.size(); ++job_cnt) {
        auto&& job       = jobs[job_cnt];
        auto&& algs      = job["algorithms"];
        unsigned alg_num = algs.size();
        std::vector<std::string> labels(alg_num);
        for (unsigned alg_cnt = 0; alg_cnt != alg_num; ++alg_cnt) {
            auto&& alg = algs[alg_cnt];
            if (alg["label"].IsDefined()) labels[alg_cnt] = "\"" + _asStr(alg["label"]) + "\"";
            else labels[alg_cnt] = "\"" + _asStr(alg["alg"]) + "\"";
        }
        std::string col1_name;
        unsigned test_num = _getTestNum(job);
        auto&& SNR        = job["SNR"];
        std::string SNR_mode;
        try {
            SNR_mode = boost::algorithm::to_lower_copy(_asStr(job["SNR_mode"], false));
        } catch (...) { SNR_mode = "db"; }
        auto&& pilot = job["pilot"];
        Value_Vec<double> SNR_vec(SNR, true);
        Value_Vec<unsigned> pilot_vec(pilot, true);
        std::string col1;
        if (SNR_vec.size() > 1) {
            col1_name = "SNR";
            if (SNR_mode != "linear") col1_name += " [dB]";
            col1 = SNR_vec.asStr(true);
        } else if (pilot_vec.size() > 1) {
            col1_name = "Pilot";
            col1      = pilot_vec.asStr(true);
        } else {
            col1_name = "Algorithm";
            col1      = "\"NMSE [dB]\"";
        }
        std::string raw_title;
        std::string title = "Job " + std::to_string(job_cnt + 1);
        if (_preCheck(job["name"], DType::STRING, false)) {
            raw_title = _asStr(job["name"]);
            title += ": " + raw_title;
        }
        _f() << "{\n"
             << "std::ofstream data_file(\"_tex_report/d" << job_cnt << ".dat\");\n"
             << "tex_file << \"\\\\simjob{" << raw_title << "}{d" << job_cnt << ".dat}{" << test_num << "}\\n\";\n"
             << "report_file << \"# " << title << "\\n\\n\";"
             << "std::string col1label = \"" << col1_name << "\";\n"
             << "std::vector<std::string> labels = {" << stringVecAsString(labels, ", ") << "};\n"
             << "std::vector<std::string> col1 = {" << col1 << "};\n"
             << "mmce::reportTable(report_file, col1label, labels, col1, 10 * arma::log10(NMSE" << job_cnt << "));\n"
             << "mmce::reportData(data_file, col1label, labels, col1, 10 * arma::log10(NMSE" << job_cnt << "));\n"
             << "report_file << \"\\n  (Simulated with " << test_num << " Monte Carlo tests.)\\n\\n\";"
             << "data_file.close();}";
    }
    _f() << "tex_file << \"\\\\simcode{" << sim_file << "}\\n\";";
    if (auto&& jobs = _config["simulation"]["jobs"]; _preCheck(jobs, DType::SEQ)) {
        std::vector<std::string> algs;
        for (auto&& job : jobs) {
            auto&& job_algs = job["algorithms"];
            if (_preCheck(job_algs, DType::SEQ)) {
                for (auto&& job_alg : job_algs) { algs.push_back(_asStr(job_alg["alg"])); }
            }
        }
        std::sort(algs.begin(), algs.end());
        algs.erase(std::unique(algs.begin(), algs.end()), algs.end());
        for (auto&& alg : algs) {
            if (auto f_name = appDir() + "/../include/mmcesim/" + alg + ".alg"; std::filesystem::exists(f_name)) {
                std::filesystem::copy_file(f_name, out_dir + "/_tex_report/" + alg + ".mmcesim-alg",
                                           std::filesystem::copy_options::overwrite_existing);
                _f() << "tex_file << \"\\\\algcode{" << alg << "}\\n\";";
            } else {
                // TODO: If the algorithm cannot be found in official library.
            }
        }
    }
    _f() << "tex_file << \"\\\\end{document}\\n\";"
         << "tex_file.close();"
         << "} else { std::cerr << \"Cannot write report!\\n\"; }"
         << "report_file.close();";
}

void Export::_ending() {
    if (lang == Lang::CPP) { _f() << "return 0;\n}\n"; }
}

bool Export::_loadALG() {
    if (auto&& jobs = _config["simulation"]["jobs"]; !_preCheck(jobs, DType::SEQ)) return false;
    else {
        // algorithms in simulation jobs
        std::vector<std::string> algs;
        for (auto&& job : jobs) {
            auto&& job_algs = job["algorithms"];
            if (!_preCheck(job_algs, DType::SEQ)) return false;
            for (auto&& job_alg : job_algs) { algs.push_back(_asStr(job_alg["alg"])); }
        }
        // functions with function CALL
        auto&& estimation_node = _config["estimation"];
        auto&& preamble_node   = _config["preamble"];
        auto&& conclusion_node = _config["conclusion"];
        auto&& appendix_node   = _config["appendix"];
        std::smatch sm;
        // Here have to find the match containing the word CALL and the space after it
        // because std::regex does not support lookbehind.
        // In my VS Code extension, I define the syntax as (?<=CALL\s+)(\w+)
        std::regex r(R"(CALL\s+\w+)");
        std::cout << "starting regex search\n";
        if (_preCheck(estimation_node, DType::STRING, false)) {
            std::string estimation_str = _asStr(estimation_node);
            while (std::regex_search(estimation_str, sm, r)) {
                algs.push_back(trim_copy(sm.str().substr(4)));
                estimation_str = sm.suffix();
                std::cout << "Regex find match: " << trim_copy(sm.str().substr(4)) << std::endl;
            }
        }
        if (_preCheck(preamble_node, DType::STRING, false)) {
            std::string preamble_str = _asStr(preamble_node);
            while (std::regex_search(preamble_str, sm, r)) {
                algs.push_back(trim_copy(sm.str().substr(4)));
                preamble_str = sm.suffix();
                std::cout << "Regex find match: " << trim_copy(sm.str().substr(4)) << std::endl;
            }
        }
        if (_preCheck(conclusion_node, DType::STRING, false)) {
            std::string conclusion_str = _asStr(conclusion_node);
            while (std::regex_search(conclusion_str, sm, r)) {
                algs.push_back(trim_copy(sm.str().substr(4)));
                conclusion_str = sm.suffix();
                std::cout << "Regex find match: " << trim_copy(sm.str().substr(4)) << std::endl;
            }
        }
        if (_preCheck(appendix_node, DType::STRING, false)) {
            std::string appendix_str = _asStr(appendix_node);
            while (std::regex_search(appendix_str, sm, r)) {
                algs.push_back(trim_copy(sm.str().substr(4)));
                appendix_str = sm.suffix();
                std::cout << "Regex find match: " << trim_copy(sm.str().substr(4)) << std::endl;
            }
        }
        std::cout << "Finished adding CALL with size " << algs.size() << "\n";
        std::sort(algs.begin(), algs.end());
        algs.erase(std::unique(algs.begin(), algs.end()), algs.end());
        for (auto&& alg : algs) {
            if (auto f_name = appDir() + "/../include/mmcesim/" + alg + ".alg"; std::filesystem::exists(f_name)) {
                std::ifstream f(f_name);
                std::stringstream buf;
                buf << f.rdbuf();
                Alg a(buf.str());
                a.write(_f(), _langStr());
            } else {
                // TODO: If the algorithm cannot be found in official library.
            }
        }
    }
    return true;
}

bool Export::_setCascadedChannel() {
    if (!_preCheck(_config["nodes"], DType::SEQ)) return false;
    auto&& nodes = _config["nodes"];
    for (int i = 0; static_cast<unsigned>(i) != nodes.size(); ++i) {
        auto&& node = nodes[i];
        auto&& role = node["role"];
        auto&& id   = node["id"];
        if (!_preCheck(id, DType::STRING)) return false; // 'id' is required for any node
        _channel_graph.nodes.push_back(id.as<std::string>());
        if (!_preCheck(role, DType::STRING | DType::UNDEF)) return false;
        if (role.IsDefined()) {
            if (auto s = boost::algorithm::to_lower_copy(role.as<std::string>());
                s == "transmitter" || s == "transmit" || s == "tx" || s == "t") {
                _transmitters.push_back(i);
                _channel_graph.Tx.push_back(i);
            } else if (s == "receiver" || s == "receive" || s == "rx" || s == "r") {
                _receivers.push_back(i);
                _channel_graph.Rx.push_back(i);
            }
        }
    }
    auto&& channels = _config["channels"];
    if (!_preCheck(channels, DType::SEQ)) return false;
    for (unsigned i = 0; i != channels.size(); ++i) {
        // Load channel matrices.
        auto&& channel = channels[i];
        auto&& id      = channel["id"];
        auto&& from    = channel["from"];
        auto&& to      = channel["to"];
        if (!_preCheck(id, DType::STRING)) return false;
        if (!_preCheck(from, DType::STRING)) return false;
        if (!_preCheck(to, DType::STRING)) return false;
        if (!_channel_graph.addChannel(id.as<std::string>(), from.as<std::string>(), to.as<std::string>()))
            return false;
    }
    if (_transmitters.size() > _MAX_TX) {
        YAML_Error e("Too many transmitters. In mmCEsim " + _MMCESIM_VER_STR + " there can be at most " +
                         std::to_string(_MAX_TX) + " transmitters.",
                     Err::TOO_MANY_TX);
        return false;
    }
    if (_receivers.size() > _MAX_RX) {
        YAML_Error e("Too many receivers. In mmCEsim " + _MMCESIM_VER_STR + " there can be at most " +
                         std::to_string(_MAX_RX) + " receivers.",
                     Err::TOO_MANY_RX);
        return false;
    }
    _channel_graph.arrange();
    std::cerr << "Channel Graph Size: " << _channel_graph.from.size() << std::endl;
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
    } catch (...) { _cascaded_channel = "H_cascaded"; }
    try {
        _received_signal = _asStr(_config["sounding"]["variables"]["received"]);
    } catch (...) { _received_signal = "y"; }
    try {
        _noise = _asStr(_config["sounding"]["variables"]["noise"]);
    } catch (...) { _noise = "n"; }
    try {
        _beamforming_F = _asStr(_config["nodes"][_transmitters[0]]["beamforming"]["variable"]);
    } catch (...) { _beamforming_F = "F"; }
    try {
        _beamforming_W = _asStr(_config["nodes"][_receivers[0]]["beamforming"]["variable"]);
    } catch (...) { _beamforming_W = "W"; }
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
    size_t BMy    = 1;
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
    try {
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
    } catch (...) {
        BMx = Mx;
        BMy = My;
    }
    return {Mx, My, GMx, GMy, BMx, BMy};
}
