#include "export.h"

Export::Export(const CLI_Options& opt) : _opt(opt) {
    std::tie(_config,_errors) = ReadConfig::read(opt.input);
    _setLang();
    if (_opt.output == "__UNDEFINED") {
        std::filesystem::path input_path = _opt.input;
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

Export::Export(const CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors) 
    : _opt(opt), _config(config), _errors(errors) {
    _setLang();
    if (_opt.output == "__UNDEFINED") {
        std::filesystem::path input_path = _opt.input;
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
    delete _f_ptr;
}

YAML_Errors Export::exportCode() {
    if (_already_error_before_export) return _errors;
    // do something
    _topComment();
    _f_ptr->close();
    return _errors;
}

YAML_Errors Export::exportCode(const CLI_Options& opt) {
    Export ep(opt);
    return ep.exportCode();
}

YAML_Errors Export::exportCode(const CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors) {
    Export ep(opt, config, errors);
    return ep.exportCode();
}

std::ofstream& Export::_f() {
    return *_f_ptr;
}

std::string Export::_langName() const {
    if (lang == Lang::CPP) return "C++ (with Armadillo library)";
    else if (lang == Lang::MATLAB) return "MATLAB";
    else if (lang == Lang::OCTAVE) return "GNU Octave";
    else if (lang == Lang::PY) return "Python (with NumPy library)";
    else if (lang == Lang::IPYNB) return "IPyNb (with NumPy library)";
    else return "Impossible branch in \"Export::_langName()\"!";
}

std::string Export::_langExtension() const {
    if (lang == Lang::CPP) return "cpp";
    else if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return "m";
    else if (lang == Lang::PY) return "py";
    else if (lang == Lang::IPYNB) return "ipynb";
    else return "Impossible branch in \"Export::_langExtension()\"!";
}

std::string Export::_langHeaderExtension() const {
    if (lang == Lang::CPP) return "h";
    else if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return "m";
    else if (lang == Lang::PY) return "py";
    else if (lang == Lang::IPYNB) return "ipynb";
    else return "Impossible branch in \"Export::_langHeaderExtension()\"!";
}

std::string Export::_langCommentSymbol() const {
    if (lang == Lang::CPP) return "//";
    else if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return "%";
    else if (lang == Lang::PY || lang == Lang::IPYNB) return "#";
    else return "Impossible branch in \"Export::_langCommentSymbol()\"!";
}

void Export::_info(const std::string& str) const {
    std::cout << "[mmCEsim] export $ " << str << std::endl;
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

template<typename T>
T Export::_as(const YAML::Node& n, bool mattered) {
    bool l;
    if (std::is_same_v<T, int>) {
        l = _preCheck(n, DType::INT, mattered);
    } else if (std::is_same_v<T, double>) {
        l = _preCheck(n, DType::DOUBLE, mattered);
    } else if (std::is_same_v<T, std::string>) {
        l = _preCheck(n, DType::STRING, mattered);
    } else if (std::is_same_v<T, bool>) {
        l = _preCheck(n, DType::BOOL, mattered);
    } else if (std::is_same_v<T, char>) {
        l = _preCheck(n, DType::CHAR, mattered);
    }
    if (l) {
        return n.as<T>();
    } else {
        throw("Invalid!");
    }
}

std::string Export::_asStr(const YAML::Node& n, bool mattered) {
    return _as<std::string>(n, mattered);
}

void Export::_setLatestError(const std::string& str) {
    assert((!_errors.empty() && "Check if errors are empty when trying to edit the last record."));
    (_errors.end() - 1)->msg = str;
}

void Export::_setLang() {
    _info(std::string("This is mmCEsim ") + _MMCESIM_VER_STR
        + ", with target version " + _config["version"].as<std::string>() + ".");
    if (auto n = _config["simulation"]; _preCheck(n, DType::MAP | DType::UNDEF)) {
        try {
            std::string lang_str = _as<std::string>(n["backend"]);
            boost::algorithm::to_lower(lang_str);
            if (lang_str == "cpp" || lang_str == "c++") lang = Lang::CPP;
            if (lang_str == "matlab" || lang_str == "m") lang = Lang::MATLAB;
            if (lang_str == "octave" || lang_str == "gnu octave") lang = Lang::OCTAVE;
            if (lang_str == "py" || lang_str == "python") lang = Lang::PY;
            if (lang_str == "ipynb") lang = Lang::IPYNB;
        } catch(...) {
            _setLatestError("'simulation->backend' is not a string"
                " (should be \"cpp\", \"matlab\", \"octave\", \"py\" or \"ipynb\").");
        }
    } else {
        _setLatestError("No simulation block defined.");
        _already_error_before_export = true;
    }
    _info("Set simulation backend as " + _langName() + ".");
}

bool Export::_isKeyword(const std::string& str) const {
    if (lang == Lang::CPP) return contains(CPP_Keywords, str);
    if (lang == Lang::MATLAB || lang == Lang::OCTAVE) return contains(MATLAB_Keywords, str);
    if (lang == Lang::PY || lang == Lang::IPYNB) return contains(PY_Keywords, str);
    else return false; // though impossible here
}

std::string Export::_asVarName(const std::string& str) const {
    if (_isKeyword(str)) return str + "_";
    else return str;
}

std::ofstream& Export::_wComment() {
    _f() << _langCommentSymbol() << ' ';
    return _f();
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
        _wComment() << "  g++ " << _opt.output << " -std=c++11 -larmadillo\n";
        _wComment() << "or\n";
        _wComment() << "  clang++ " << _opt.output << " -std=c++11 -larmadillo\n";
        _wComment() << "or just link to Armadillo library with whatever compiler you have.\n";
    }
    _f() << "\n";
}
