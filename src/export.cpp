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
    _generateChannels();
    _algorithms();
    _sounding();
    _estimation();
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
    std::ifstream header_file(appDir() + "/../include/mmcesim/copy/header." + _langMmcesimExtension());
    std::string header_content = "";
    while (!header_file.eof()) {
        header_content += header_file.get();
    }
    header_content.erase(header_content.end() - 1); // last read character is invalid, erase it
    _f() << header_content << '\n';
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
    if (lang == Lang::CPP) {
        _f() << "namespace mmce {\nbool generateChannels() {" << '\n';
        std::cout << "Tx index: " << _transmitters[0] << ", Rx index: " << _receivers[0] << '\n';
        auto&& t_node = _config["nodes"][_transmitters[0]];
        auto&& r_node = _config["nodes"][_receivers[0]];
        auto&& t_size = t_node["size"];
        auto&& r_size = r_node["size"];
        // std::cout << r_size << std::endl;
        // std::cout << t_size["size"][0] << '\n';
        // TODO: YAML check here
        // std::string M_str = r_size.as<std::string>();
        Value_Vec<size_t> M_value_vec(r_size, false, 1);
        size_t Mx = M_value_vec[0];
        size_t My = M_value_vec[1];
        size_t GMx;
        size_t GMy = 1;
        auto&& r_grid = r_node["grid"];
        // std::string GM_str = r_grid.as<std::string>();
        try {
            std::string GM_str = r_grid.as<std::string>();
            if (boost::algorithm::to_lower_copy(GM_str) == "same") {
                GMx = Mx;
                GMy = My;
            } else {
                Value_Vec<size_t> GM_value_vec(r_grid, false, 1);
                GMx = GM_value_vec[0];
                GMy = GM_value_vec[1];
            }
        } catch (...) {
            Value_Vec<size_t> GM_value_vec(r_grid, false, 1);
            GMx = GM_value_vec[0];
            GMy = GM_value_vec[1];
        }
        Value_Vec<size_t> N_value_vec(t_size, false, 1);
        size_t Nx = N_value_vec[0];
        size_t Ny = N_value_vec[1];
        size_t GNx;
        size_t GNy = 1;
        auto&& t_grid = t_node["grid"];
        // std::string GM_str = r_grid.as<std::string>();
        try {
            std::string GN_str = t_grid.as<std::string>();
            if (boost::algorithm::to_lower_copy(GN_str) == "same") {
                GNx = Nx;
                GNy = Ny;
            } else {
                Value_Vec<size_t> GN_value_vec(t_grid, false, 1);
                GNx = GN_value_vec[0];
                GNy = GN_value_vec[1];
            }
        } catch (...) {
            Value_Vec<size_t> GN_value_vec(t_grid, false, 1);
            GNx = GN_value_vec[0];
            GNy = GN_value_vec[1];
        }
        _f() << "mmce::channel("
             << Mx << "," << My << ","
             << Nx << "," << Ny << ","
             << GMx << "," << GMy << ","
             << GNx << "," << GNy << ","
             << sparsity << ","
             << gain_normal << "," << gain_param1 << "," << gain_param2 << ","
             << off_grid
             << ");";
        _f() << "}}\n\n";
    }
    // TODO: Generate channels.
}

void Export::_algorithms() {
    // search through all used algorithms,
    // and generate them here as a function.
}

void Export::_sounding() {
    if (lang == Lang::CPP) {
        _f() << "int main(int argc, char* argv[]) {\n"
             << "arma_rng::set_seed_random();\n"
             << "mmce::generateChannels();\n";
    }
    // so far, the test of Alg implementation will be here.
}

void Export::_estimation() {
    std::string sounding_str;
    if (!_preCheck(_config["estimation"], DType::STRING)) {
        // maybe add a message on the console
        sounding_str = "auto";
    } else {
        sounding_str = _asStr(_config["estimation"]);
    }
    trim(sounding_str);
    std::cout << sounding_str << "\n";
    if (sounding_str.length() == 4 && boost::algorithm::to_lower_copy(sounding_str) == "auto") {
        // TODO: use tasked-oriented algorithm
        // just do nothing right now
        std::cout << "Use auto sounding." << std::endl;
    } else {
        Alg alg(sounding_str);
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
    auto nodes = _config["nodes"];
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
