#include "export.h"

Export::Export(const std::string& file) {
    std::tie(_config, _errors) = ReadConfig::read(file);
    _setLang();
}

Export::Export(const YAML::Node& config, const YAML_Errors& errors) 
    : _config(config), _errors(errors) {
    _setLang();
}

YAML_Errors Export::exportCode() {
    return _errors;
}

YAML_Errors Export::exportCode(const std::string& file) {
    Export ep(file);
    return ep.exportCode();
}

YAML_Errors Export::exportCode(const YAML::Node& config, const YAML_Errors& errors) {
    Export ep(config, errors);
    return ep.exportCode();
}

void Export::_setLang() {

}
