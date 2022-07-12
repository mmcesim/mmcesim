#include "export.h"

Export::Export(const CLI_Options& opt) {
    std::tie(_config, _errors) = ReadConfig::read(opt.input);
    _setLang();
}

Export::Export(const CLI_Options& opt, const YAML::Node& config, const YAML_Errors& errors) 
    : _config(config), _errors(errors) {
    _setLang();
}

YAML_Errors Export::exportCode() {
    if (_already_error_before_export) return _errors;
    // do something
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

std::string Export::_langName() const {
    if (lang == Lang::CPP) return "C++ (with Armadillo library)";
    else if (lang == Lang::MATLAB) return "MATLAB";
    else if (lang == Lang::OCTAVE) return "GNU Octave";
    else if (lang == Lang::PY) return "Python (with NumPy library)";
    else if (lang == Lang::IPYNB) return "IPyNb (with NumPy library)";
    else return "Impossible branch in \"Export::_langName()\"!";
}

void Export::_info(const std::string& str) const {
    std::cout << "[mmCEsim] export $ " << str << std::endl;
}

bool Export::_preCheck(const YAML::Node& n, unsigned a_t) {
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
        // This is the general error message,
        // but in use, the error message can be more specific
        // by using method _setLatestError.
        std::string msg = errorMsg(Err::YAML_DTYPE) + " (" + n.Tag() + ")";
        YAML_Error e(msg, Err::YAML_DTYPE);
        _errors.push_back(e);
        return false;
    }
}

template<typename T>
T Export::_as(const YAML::Node& n) {
    bool l;
    if (std::is_same_v<T, int>) {
        l = _preCheck(n, DType::INT);
    } else if (std::is_same_v<T, double>) {
        l = _preCheck(n, DType::DOUBLE);
    } else if (std::is_same_v<T, std::string>) {
        l = _preCheck(n, DType::STRING);
    } else if (std::is_same_v<T, bool>) {
        l = _preCheck(n, DType::BOOL);
    } else if (std::is_same_v<T, char>) {
        l = _preCheck(n, DType::CHAR);
    }
    if (l) {
        return n.as<T>();
    } else {
        throw("Invalid!");
    }
}

void Export::_setLatestError(const std::string& str) {
    assert((!_errors.empty() & "Check if errors are empty when trying to edit the last record."));
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
