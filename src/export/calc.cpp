/**
 * @file calc.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Calc Class
 * @version 0.1.0
 * @date 2022-08-20
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include "export/calc.h"

Calc::Calc(const std::string& str) : _str(removeSpaceCopy(str)) {}

std::string Calc::as(std::string lang, std::string* msg) const {
    if (_str.empty()) return "";
    std::string str = _str;
    if (_changeOperator(str, lang, msg) &&
        _changeFunction(str, lang, msg) &&
        _changeSuperScript(str, lang, msg) &&
        _changeSubScript(str, lang, msg) &&
        _changeParen(str, lang, msg)) {}
    return str;
}

#define LANG_CPP if (lang == "cpp") {
#define LANG_PY } else if (lang == "py") {
#define LANG_M } else if (lang == "matlab" || lang == "octave") {
#define END_LANG } else throw("Unknown language " + lang + ".");

bool Calc::_changeOperator(std::string& str, std::string lang, std::string* msg) const {
    LANG_CPP
        boost::replace_all(str, ".*", "%"); // element-wise multiplication
        boost::replace_all(str, "./", "/"); // element-wise division
        boost::replace_all(str, "@", "*"); // multiplication
    END_LANG
    return true;
}

bool Calc::_changeFunction(std::string& str, std::string lang, std::string* msg) const {
    // Note that the sequences here matter
    // for example exp10 should be replaced before exp
    // though the result currently is the same now,
    // but the logic is right in replacing what you need
    // and should be safer.
    LANG_CPP
        _addArma(str, "expm1");
        _addArma(str, "exp10");
        _addArma(str, "exp2");
        _addArma(str, "exp");
        _addArma(str, "log1p");
        _addArma(str, "log10");
        _addArma(str, "log2");
        _addArma(str, "log");
        _addArma(str, "pow");
        _addArma(str, "square");
        _addArma(str, "sqrt");
        _addArma(str, "floor");
        _addArma(str, "ceil");
        _addArma(str, "round");
        _addArma(str, "trunc");
        _addArma(str, "erf");
        _addArma(str, "erfc");
        _addArma(str, "tgamma");
        _addArma(str, "lgamma");
        _addArma(str, "abs");
        _addArma(str, "cosh");
        _addArma(str, "cos");
        _addArma(str, "acosh");
        _addArma(str, "acos");
        _addArma(str, "sinh");
        _addArma(str, "sinc");
        _addArma(str, "sin");
        _addArma(str, "asinh");
        _addArma(str, "sinh");
        _addArma(str, "tanh");
        _addArma(str, "tan");
        _addArma(str, "atanh");
        _addArma(str, "atan2");
        _addArma(str, "tanh");
        _addArma(str, "inv");
        _addArma(str, "conj");
        _addArma(str, "pinv");
        _addArma(str, "sign");
        boost::replace_all(str, "\\sgn", "arma::sign");
    END_LANG
    return true;
}

bool Calc::_changeSuperScript(std::string& str, std::string lang, std::string* msg) const {
    LANG_CPP
        boost::replace_all(str, "^t", ".t()"); // transpose for a real matrix
        boost::replace_all(str, "^{t}", ".t()"); // transpose for a real matrix
        boost::replace_all(str, "^T", ".st()"); // transpose for a complex matrix
        boost::replace_all(str, "^{T}", ".st()"); // transpose for a complex matrix
        boost::replace_all(str, "^H", ".t()"); // conjugate transpose for a complex matrix
        boost::replace_all(str, "^{H}", ".t()"); // conjugate transpose for a complex matrix
        boost::replace_all(str, "^i", ".i()"); // inverse of a complex matrix
        boost::replace_all(str, "^I", ".i()"); // inverse of a complex matrix
        boost::replace_all(str, "^{-1}", ".i()"); // inverse of a complex matrix
        boost::replace_all(str, "^*", ".t().h()"); // conjugate of a complex matrix
        boost::replace_all(str, "^{*}", ".t().h()"); // conjugate of a complex matrix
        boost::replace_all(str, "^\\star", ".t().h()"); // conjugate of a complex matrix
        boost::replace_all(str, "^{\\star}", ".t().h()"); // conjugate of a complex matrix
        boost::replace_all(str, "^\\ast", ".t().h()"); // conjugate of a complex matrix
        boost::replace_all(str, "^{\\ast}", ".t().h()"); // conjugate of a complex matrix
    LANG_PY
    LANG_M
    END_LANG
    // for (size_t i = 0; i != str.size(); ++i) {
    //     if (str[i] == '^') {
    //         if (i + 1 == str.size()) {
    //             _msg(msg, "Trailing ^.");
    //             return false;
    //         } else {
    //             if (char c = str[++i]; c == 't') {
    //                 // transpose for a real matrix
    //                 str[i - 1] = '.';
    //                 str[i] = 's';
    //             }
    //         }
    //     }
    // }
    return true;
}

bool Calc::_changeSubScript(std::string& str, std::string lang, std::string* msg) const {
    for (size_t i = 0; i != str.size(); ++i) {
        if (str[i] == '_') {
            size_t start_i = i;
            std::string subs = "(";
            if (i + 1 == str.size()) {
                // Since we are using '_' as subscript,
                // we do not allow variable names end with '_'.
                _msg(msg, "Trailing _.");
                return false;
            } else {
                if (char c = str[i + 1]; c == '{') {
                    if (i + 2 == str.size()) {
                        _msg(msg, "Trailing {.");
                        return false;
                    } else if (str[i + 2] == '}') {
                        // like: _{}, so nothing happens
                        i += 2;
                        continue;
                    } else i += 2; // skip '_{'
                    std::string dims[3];
                    bool starts_from_1[3] = { false, false, false };
                    int dim = 0;
                    while (i != str.size()) {
                        char ch = str[i];
                        if (ch == ',') {
                            ++dim;
                            if (dim > 2) {
                                _msg(msg, "Too many dimensions in subscript.");
                                return false;
                            } else if (i + 1 == str.size()) {
                                _msg(msg, "Unclosed {.");
                                return false;
                            } else {
                                if (str[i + 1] == '&') {
                                    starts_from_1[dim] = true;
                                    ++i;
                                }
                            }
                        } else if (ch == '}') {
                            break;
                        } else {
                            dims[dim] += ch;
                        }
                        ++i;
                    }
                    for (int j = 0; j <= dim; ++j) {
                        size_t colon_pos = dims[j].find(':');
                        bool has_colon = colon_pos != std::string::npos;
                        // size_t question_pos = dims[j].find('?');
                        // bool has_question = question_pos != std::string::npos;
                        if (has_colon) {
                            if (dims[j] == ":") {
                                LANG_CPP
                                    subs += "arma::span::all";
                                LANG_PY
                                LANG_M
                                    subs += ":";
                                END_LANG
                            } else {
                                subs += "arma::span(" + dims[j].substr(0, colon_pos) + "," +
                                        dims[j].substr(colon_pos + 1) + ")";
                            }
                        } else {
                            subs += dims[j];
                        }
                        if (j != dim) subs += ',';
                            else subs += ')';
                    }
                    str.replace(start_i, i, subs);
                } // otherwise it might just be part of variable name
            }
        }
    }
    return true;
}

bool Calc::_changeParen(std::string& str, std::string lang, std::string* msg) const {
    LANG_CPP
        boost::replace_all(str, "{", "("); // element-wise multiplication
        boost::replace_all(str, "}", ")"); // element-wise division
    END_LANG
    return true;
}

#undef LANG_CPP
#undef LANG_PY
#undef LANG_M
#undef END_LANG
