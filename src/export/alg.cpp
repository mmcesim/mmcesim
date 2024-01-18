/**
 * @file alg.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of class Alg
 * @version 0.2.2
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2022-2024 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "export/alg.h"

Alg::Alg(const std::string& str, const Macro& macro, int job_cnt, int alg_cnt, bool fail_fast, bool add_comment,
         bool add_semicolon, ALG_Opt opt)
    : _macro(macro), _failed(false), _job_cnt(job_cnt), _add_semicolon(add_semicolon), _add_comment(add_comment) {
    std::string str_replaced_macro = macro.replaceMacro(str, job_cnt, alg_cnt);
    std::stringstream ss(str_replaced_macro);
    std::string line;
    std::string unterminated_line = "";
    Alg_Lines::size_type line_no  = 0;
    while (std::getline(ss, line, '\n')) { // separate by newline
        ++line_no;
        trim(line);                 // remove the whitespace around it.
        if (line.empty()) continue; // skip empty line
        // bool unterminated_here = *(line.end() - 1) == '\\';
        if (*(line.end() - 1) == '\\') {
            // The last character is backslash
            // so the line has not terminated.
            unterminated_line += std::string(" ") + line;
        } else {
            auto s = unterminated_line + line;
            try {
                Alg_Line l(s, opt);
                if (!_failed) {
                    // If the status of 'failed' is true,
                    // the only aim of keeping parsing these lines
                    // is to check the syntax.
                    // No matter what happens later,
                    // the lines are not useable.
                    // Therefore, we only push line when we have not failed.
                    _lines.push_back(l);
                    _raw_strings.push_back(s);
                    _line_nos.push_back(line_no);
                }
            } catch (const std::runtime_error& e) {
                if (fail_fast) break;
                _failed = true;
                _errors.push_back({ e.what(), s, line_no });
            }
            unterminated_line.clear();
        }
    }
}

#define SWITCH_FUNC if (false) {
#define CASE(_func_name__)                                                                                             \
    }                                                                                                                  \
    else if (func == _func_name__) {
#define DEFAULT                                                                                                        \
    }                                                                                                                  \
    else {
#define END_SWITCH }
#define LANG_CPP   if (lang == "cpp") {
#define LANG_PY                                                                                                        \
    }                                                                                                                  \
    else if (lang == "py") {
#define LANG_M                                                                                                         \
    }                                                                                                                  \
    else if (lang == "matlab" || lang == "octave") {
#define END_LANG                                                                                                       \
    }                                                                                                                  \
    else throw("Unknown language " + lang + ".");
#define ERROR(msg)   _errors.push_back({ msg, _raw_strings[i], _line_nos[i] })
#define WARNING(msg) _warnings.push_back({ msg, _raw_strings[i], _line_nos[i] })
#define INDENT       _indent(_indent_size)
#define _m(_str__)   _macro.replaceMacro(_str__, _job_cnt, _alg_cnt)
#define _mi(_i__)    _m(line.params(_i__).value)
#define _ms(_s__)    _m(line[_s__])

// Apply keys and check repeated/unknown keys.
#define APPLY_KEYS(_func_name__)                                                                                       \
    if (!_applyKey(line, keys)) ERROR("More parameters than expected in '"s + _func_name__ + "'.");                    \
    if (line.hasRepeatedKey()) ERROR("Repeated key in '"s + _func_name__ + "'.");                                      \
    if (line.hasUnknownKey(keys, unknown_key))                                                                         \
        ERROR("Unknown key '"s + unknown_key + "' in Function '"s + _func_name__ + "'.");

bool Alg::write(std::ofstream& f, const std::string& lang) {
    size_t indent_cnt = 0;                    // used for Python and MATLAB.
    for (int i = 0; i < _lines.size(); ++i) { // use i because sometimes it will be -1 before adding 1.
        Alg_Line line           = _lines[i];
        const std::string& func = line.func();
        bool is_func_declare    = line.isFunctionDeclaration();
        if (is_func_declare) {
            if (func.empty()) continue;
            else if (func != "FUNCTION") {
                _log.info() << "ALG ignoring function '" << func << "' (in function declaration)" << std::endl;
                continue;
            } else _log.info() << "ALG writing function declaration '" << func << "'" << std::endl;
        }
        line.print(_log.write(), "[INFO] ");
        std::string unknown_key;
        // clang-format off
        SWITCH_FUNC
            // function no end
            CASE ("BRANCH")
                f << '\n';
                _wComment(f, lang, INDENT) << "[" << _alg_cnt + 1 << "/" << _macro.alg_num[_job_cnt]
                    << "] Algorithm: " << _macro.alg_names[_job_cnt][_alg_cnt] << '\n';
                _log.info() << "Job (" << _job_cnt + 1 << "/" << _macro.job_num << ") "
                            << "Algorithm (" << _alg_cnt + 1 << "/" << _macro.alg_num[_job_cnt] << ") '"
                            << _macro.alg_names[_job_cnt][_alg_cnt] << "'" << std::endl;
                _branch_line = i;
                // TODO: Consider the 'metric' field, so far we only consider it as NMSE
                LANG_CPP
                    // f << "vec sim_NMSE(" << _macro.alg_num[_job_cnt] << ", arma::fill::zeros);\n";
                    f << "{";
                END_LANG
                _recover_cnt = 0;
                type_track++;
            CASE ("BREAK")
                f << "break";
                LANG_CPP
                    if (_add_semicolon) f << ";\n";
                END_LANG
            CASE ("CALC")
                std::string msg;
                std::string out;
                LANG_CPP
                    if (line.params().size() == 0) {
                        if (_add_semicolon) f << ";\n";
                    } else {
                        out = Calc::as(_mi(0), "cpp", &msg);
                        if (msg.empty()) {
                            if (size_t s = line.returns().size(); s != 0) {
                                if (s == 1) {
                                    f << inlineCalc(line.returns(0).name, "cpp");
                                } else {
                                    // TODO: multiple return values
                                }
                                f << "=";
                            }
                            f << out;
                            if (_add_semicolon) f << ";\n";
                        } else {
                            std::cerr << msg << "\n";
                            // TODO: handle error here
                        }
                    }
                LANG_PY
                LANG_M
                END_LANG
            CASE ("CALL")
                Keys keys { "name", "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9", "init" };
                APPLY_KEYS("CALL");
                LANG_CPP
                    std::string return_type;
                    // auto paramType = [] (const Type& type) -> std::string {
                    //     if (type.dim() > 0) {
                    //         return std::string(type.isConst() ? "" : "const ") +
                    //             type.string() + (type.isReference() ? "" : "&");
                    //     } else {
                    //         return type.string();
                    //     }
                    // };
                    if (line.returns().size() > 0) {
                        if (line.hasKey("init") && _ms("init") != "false" && _ms("init") != "0") {
                            if (size_t s = line.returns().size(); s == 0) {
                                return_type = "";
                            } else if (s == 1) {
                                auto&& type = line.returns(0).type;
                                return_type = type.empty() ? "auto " : static_cast<Type>(type).string() + " ";
                            } else {
                                // TODO: multiple return values
                            }
                            f << return_type;
                        }
                        f <<  line.returns(0).name <<  "=";
                    }
                    f << _mi(0) << "(";
                    unsigned p = 10; // the number of parameters
                    while (--p != 0) {
                        if (line.hasKey(std::string("p" + std::to_string(p)))) break;
                    }
                    if (p > 0) {
                        for (unsigned i = 1; i != p; ++i) {
                            // TODO: check type if specified
                            f << inlineCalc(_mi(i), "cpp") << ",";
                        }
                        f << inlineCalc(_mi(p), "cpp");
                    }
                    f << ")";
                    if (_add_semicolon) f << ";\n";
                END_LANG
            CASE ("COMMENT")
                std::string comment;
                if (line.params().size() > 1) {
                    for (auto&& s : line.params()) comment += _m(s.value) + ' ';
                } else if (line.params().empty()) WARNING("Empty comment content.");
                else comment = line.params(0).value;
                trim(comment);
                // if (isQuoted(comment))
                //     _wComment(f, lang, INDENT) << std::quoted(comment) << '\n';
                // else _wComment(f, lang, INDENT) << comment << '\n';
                _wComment(f, lang, INDENT) << removeQuote(comment) << '\n';
            CASE ("CPP")
                LANG_CPP
                    std::string cpp_content;
                    if (!line.params().empty()) {
                        cpp_content = _mi(0);
                        f << cpp_content;
                    }
                    if (!cpp_content.empty() && *(cpp_content.cend() - 1) != ';' && _add_semicolon) {
                        f << ";\n";
                    }
                END_LANG
            CASE ("ESTIMATE")
                if (line.returns().empty()) WARNING("No return value given!.");
                else {
                    Keys keys { "Q", "y", "nonezero", "init" };
                    APPLY_KEYS("ESTIMATE");
                    std::string estimate_str = line.returns(0).name + "::";
                    if (auto&& type = line.returns(0).type; type.empty()) estimate_str += "v";
                    else estimate_str += type;
                    if (_macro.alg_names[_job_cnt][_alg_cnt] == "Oracle_LS") {
                        try {
                            estimate_str += " = CALL Oracle_LS " + _ms("Q") + " " + _ms("y") + " "
                                + inlineCalc(_ms("nonezero"), "cpp") + " " + _macro.alg_params[_job_cnt][_alg_cnt];
                        } catch (...) {
                            ERROR("No 'nonezero' parameter specified in 'ESTIMATE'.");
                        }
                    } else {
                        estimate_str += " = CALL " + _macro.alg_names[_job_cnt][_alg_cnt] +
                            " " + _ms("Q") + " " + _ms("y") + " " + _macro.alg_params[_job_cnt][_alg_cnt];
                    }
                    if (line.hasKey("init")) estimate_str += std::string(" init=") + line["init"];
                    Alg estimate_alg(estimate_str, _macro, _job_cnt, _alg_cnt);
                    estimate_alg.write(f, lang);
                }
            CASE ("INIT")
                // std::cout << "I am in INIT" << std::endl;
                // for (auto&& param : line.params()) {
                //     std::cout << param.value << '\n';
                // }
                if (line.returns().size() > 1) ERROR("Return variable more than 1 in 'INIT'.");
                else if (line.returns().empty()) WARNING("Unused 'INIT', i.e. no return variable.");
                else {
                    Keys keys { "dim1", "dim2", "dim3", "fill", "scale", "dtype" };
                    APPLY_KEYS("INIT");
                    auto getReturnType = [&line] (char dim) {
                        if (auto&& s = line.returns(0).type; !s.empty()) {
                            // 'dtype' is ignored if the return type is specified
                            return s;
                        } else {
                            return (line.hasKey("dtype") ?
                                    removeQuote(line["dtype"]) :
                                    std::string("c")) + dim;
                        }
                    };
                    auto cppScaleFill = [this, &line, &f] (const Type& t) -> std::string {
                        f << t.string() << " " << line.returns(0).name << " = ";
                        if (t.dim() > 0) {
                            if (line.hasKey("scale")) {
                                f << '(' << inlineCalc(_ms("scale"), "cpp") << ") * ";
                            }
                            std::string fill = "zeros";
                            if (line.hasKey("fill")) {
                                fill = boost::algorithm::to_lower_copy(line["fill"]);
                            }
                            return fill + "<" + t.string() + ">";
                        } else if (t.dim() == 0) {
                            std::string fill = "0";
                            if (line.hasKey("fill")) {
                                auto&& s = line["fill"];
                                if (s == "zeros") {
                                    fill = "0";
                                } else if (s == "ones") {
                                    if (line.hasKey("scale")) fill = inlineCalc(_ms("scale"), "cpp");
                                    else fill = "1";
                                } else {
                                    f << '(' << inlineCalc(_ms("scale"), "cpp") << ") * ";
                                    fill = std::string("arma::") + line["fill"];
                                    // TODO: check if it is randn or randi
                                }
                            }
                            return fill + "<" + t.string() + ">";
                        } else {
                            // TODO: handle error of unknown dimension
                            return "";
                        }
                    };
                    if (line.hasKey("dim1")) {
                        if (line.hasKey("dim2")) {
                            if (line.hasKey("dim3")) {
                                // dim: 3 (a tensor)
                                Type type = getReturnType('3');
                                LANG_CPP
                                    auto fill = cppScaleFill(type);
                                    f << "arma::" << fill << '(' << inlineCalc(_ms("dim1"), "cpp") << ", "
                                        << inlineCalc(_ms("dim2"), "cpp") << ", "
                                        << inlineCalc(_ms("dim3"), "cpp") << ")";
                                END_LANG
                                type_track.push(line.returns(0).name, type);
                            } else {
                                // dim: 2 (a matrix)
                                Type type = getReturnType('2');
                                LANG_CPP
                                    auto fill = cppScaleFill(type);
                                    f << "arma::" << fill << '(' << inlineCalc(_ms("dim1"), "cpp")
                                    << ", " << inlineCalc(_ms("dim2"), "cpp") << ")";
                                END_LANG
                                type_track.push(line.returns(0).name, type);
                            }
                        } else {
                            // dim: 1 (a vector)
                            Type type = getReturnType('1');
                            LANG_CPP
                                auto fill = cppScaleFill(type);
                                if (auto&& s = line.returns(0).type; s == "r") {
                                    // If it is a row vector,
                                    // the user may only specify one dimension.
                                    // But it should be understood as a matrix now.
                                    f << "arma::" << fill << "(1, " << inlineCalc(_ms("dim1"), "cpp") << ")";
                                    type_track.push(line.returns(0).name, type);
                                } else if (Type type_ = s; type_.dim() == 0) {
                                    // scalar assigning can just use the 
                                    if (line.hasKey("scale")) {
                                        f << inlineCalc(_ms("scale"), "cpp") << "";
                                    } else {
                                        f << inlineCalc(_ms("dim1"), "cpp") << "";
                                    }
                                    type_track.push(line.returns(0).name, type_);
                                } else {
                                    f << "arma::" << fill << '('
                                      << inlineCalc(_ms("dim1"), "cpp") << ")";
                                    type_track.push(line.returns(0).name, type);
                                }
                            END_LANG
                        }
                    } else {
                        // dim: 0 (a scalar)
                        Type type = getReturnType('0');
                        LANG_CPP
                            f << type.string() << " " << line.returns(0).name;
                        END_LANG
                        type_track.push(line.returns(0).name, type);
                    }
                    LANG_CPP
                        if (_add_semicolon) f << ";\n";
                    END_LANG
                }
            CASE ("MATLAB")
            CASE ("MERGE")
                ++_alg_cnt;
                if (_branch_line != Alg::max_length && _alg_cnt < _macro.alg_num[_job_cnt]) {
                    // meaning the last while BRANCH is not closed
                    i = _branch_line - 1;
                } else if (_alg_cnt == _macro.alg_num[_job_cnt]) {
                    _branch_line = Alg::max_length;
                }
                if (_recover_cnt == 0) {
                    WARNING(fmt::format("No 'RECOVER' function found in estimation (Job: {}, Alg: {}).",
                        _job_cnt + 1, _alg_cnt + 1));
                    _log.war() << "No 'RECOVER' function found in estimation (Job: " << _job_cnt + 1
                               << ", Alg: " << _alg_cnt << ")." << std::endl;
                } else {
                    _log.info() << "Found " << _recover_cnt << " 'RECOVER' function(s) in estimation (Job: "
                                << _job_cnt + 1 << ", Alg: " << _alg_cnt << ")." << std::endl;
                }
                LANG_CPP
                    if (_recover_cnt > 1) {
                        f << "NMSE" << _job_cnt << "(ii, " << _alg_cnt - 1  << ") /= " << _recover_cnt << ";\n";
                    }
                    f << "}";
                END_LANG
                _recover_cnt = 0;
                try {
                    type_track--;
                } catch (const std::out_of_range& e) {
                    std::cerr << "ERROR: " << __FILE__ << " " << __LINE__
                              << ": " << e.what() << std::endl; 
                }
            CASE ("NEW")
                std::string msg;
                std::string out;
                LANG_CPP
                    if (line.params().size() == 0) {
                        if (_add_semicolon) f << ";\n";
                    } else {
                        out = Calc::as(_mi(0), "cpp", &msg);
                        if (msg.empty()) {
                            if (size_t s = line.returns().size(); s != 0) {
                                if (s == 1) {
                                    auto&& type = line.returns(0).type;
                                    f << (type.empty() ? "auto " : static_cast<Type>(type).string() + " ")
                                      << line.returns(0).name;
                                    type_track.push(line.returns(0).name, type);
                                } else {
                                    // TODO: multiple return values
                                }
                                f << "=";
                            }
                            f << out;
                            if (_add_semicolon) f << ";\n";
                        } else {
                            std::cerr << msg << "\n";
                            // TODO: handle error here
                        }
                    }
                LANG_PY
                LANG_M
                END_LANG
            CASE ("OCTAVE")
            CASE ("PRINT")
                // TODO: set stream type and return file
                LANG_CPP
                    f << "std::cout";
                    for (size_t j = 0; j != line.params().size(); ++j) {
                        f << " << " << inlineCalc(_mi(j), lang);
                    }
                    if (_add_semicolon) f << ";\n";
                LANG_PY
                LANG_M
                END_LANG
            CASE ("RECOVER")
                if (!line.returns().empty()) WARNING("No return value is needed in function 'RECOVER'!.");
                else {
                    Keys keys { "H" };
                    APPLY_KEYS("RECOVER");
                    std::string recover_str = "= NMSE" + std::to_string(_job_cnt) + "_{ii," +
                                              std::to_string(_alg_cnt) + "} += \\nmse(" +
                                              line["H"] + ", " + _macro._cascaded_channel + ")";
                    Alg recover_alg(recover_str, _macro, _job_cnt, _alg_cnt);
                    recover_alg.write(f, lang);
                }
                ++_recover_cnt;
            // function needs end
            CASE ("ELSE")
                LANG_CPP
                    f << "} else {";
                END_LANG
                try {
                    type_track--;
                } catch (const std::out_of_range& e) {
                    std::cerr << "ERROR: " << __FILE__ << " " << __LINE__
                              << ": " << e.what() << std::endl;
                }
                type_track++;
            CASE ("ELIF")
                Keys keys { "cond" };
                APPLY_KEYS("ELIF");
                LANG_CPP
                    f << "} else if (";
                    if (line.hasKey("cond")) {
                        Alg cond(_ms("cond"), macro_none, -1, -1, false, false, false);
                        cond.write(f, "cpp");
                    } else {
                        // TODO: handle error when no contents specified
                    }
                    f << ") {";
                END_LANG
                try {
                    type_track--;
                } catch (const std::out_of_range& e) {
                    std::cerr << "ERROR: " << __FILE__ << " " << __LINE__
                              << ": " << e.what() << std::endl; 
                }
                type_track++;
            CASE ("FOR")
                Keys keys { "init", "cond", "oper" };
                APPLY_KEYS("FOR");
                // init call INIT/CALC function
                LANG_CPP
                    f << "for (";
                    // std::cout << "line[init] = " << line["init"] << '\n';
                    if (line.hasKey("init")) {
                        if (inlineCalc(_ms("init"), lang) == "") {
                            f << ";";
                        } else {
                            Alg init(inlineCalc(_ms("init"), lang), macro_none, -1, -1, false, false, true);
                            init.write(f, "cpp");
                        }
                    } else f << ";";
                    if (line.hasKey("cond")) {
                        Alg cond(inlineCalc(_ms("cond"), lang), macro_none, -1, -1, false, false, true);
                        cond.write(f, "cpp");
                    } else f << ";";
                    if (line.hasKey("oper")) {
                        Alg oper(inlineCalc(_ms("oper"), lang), macro_none, -1, -1, false, false, false);
                        oper.write(f, "cpp");
                        // std::cout << "has oper!\n";
                    }
                    f << ") {";
                END_LANG
                type_track++;
                ++indent_cnt;
                _contents_at_end.push("");
            CASE ("FOREVER")
                LANG_CPP
                    f << "while(1) {";
                END_LANG
                type_track++;
                ++indent_cnt;
                _contents_at_end.push("");
            CASE ("FUNCTION")
                // It is worth noting that FUNCTION should only exist in preamble
                // because in C++ there is no nested function (should use lambda expression instead).
                Keys keys { "name", "p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8", "p9" };
                APPLY_KEYS("FUNCTION");
                LANG_CPP
                    std::string return_type;
                    auto paramType = [] (const Type& type) -> std::string {
                        if (type.dim() > 0) {
                            return std::string(type.isConst() ? "" : "const ") +
                                type.string() + (type.isReference() ? "" : "&");
                        } else {
                            return type.string();
                        }
                    };
                    size_t s = line.returns().size();
                    if (s == 0) {
                        return_type = "void ";
                    } else if (s == 1) {
                        auto&& type = line.returns(0).type;
                        return_type = type.empty() ? "auto " : static_cast<Type>(type).string() + " ";
                    } else {
                        auto&& type = line.returns(0).type;
                        return_type = type.empty() ? "auto " : static_cast<Type>(type).string() + " ";
                        // TODO: multiple return values
                    }
                    f << return_type << _mi(0) << "(";
                    unsigned p = 10; // the number of parameters
                    while (--p != 0) {
                        if (line.hasKey("p" + std::to_string(p))) break;
                    }
                    if (p > 0) {
                        for (unsigned i = 1; i != p; ++i) {
                            // TODO: check type if specified
                            f << paramType(line.params(i).type) << " " << _mi(i) << ",";
                        }
                        f << paramType(line.params(p).type) << " " << _mi(p);
                    }
                    if (is_func_declare) {
                        f << ");\n";
                    } else {
                        if (s > 0) {
                            f << ") {\n" << return_type << line.returns(0).name << ";\n";
                            _contents_at_end.push("return " + line.returns(0).name + ";");
                        } else f << ") {\n";
                    }
                END_LANG
                type_track++;
                ++indent_cnt;
            CASE ("IF")
                Keys keys { "cond" };
                APPLY_KEYS("IF");
                LANG_CPP
                    f << "if (";
                    if (line.hasKey("cond")) {
                        Alg cond(removeQuote(_mi("cond")), macro_none, -1, -1, false, false, false);
                        cond.write(f, "cpp");
                    } else {
                        // TODO: handle error when no contents specified
                    }
                    f << ") {";
                END_LANG
                type_track++;
                ++indent_cnt;
                _contents_at_end.push("");
            CASE ("LOG")
                std::string text;
                if (line.params().size() > 1) {
                    for (auto&& s : line.params()) text += _m(s.value) + ' ';
                } else if (line.params().empty()) WARNING("Empty LOG content.");
                else text = line.params(0).value;
                trim(text);
                _log.write() << removeQuote(text) << std::endl;
            CASE ("LOOP")
                type_track++;
                Keys keys { "begin", "end", "step", "from", "to" };
                APPLY_KEYS("LOOP");
                LANG_CPP
                    std::string var_name = "i";
                    f << "for (";
                    if (!line.returns().empty()) {
                        if (auto&& s = line.returns(0).type; !s.empty()) {
                            Type iter_type = s;
                            f << iter_type.string() << ' ';
                            type_track.push(line.returns(0).name, iter_type);
                        } else {
                            f << "auto ";
                            type_track.push(line.returns(0).name, "u0");
                        }
                        var_name = line.returns(0).name;
                        f << var_name;
                    } else {
                        f << "auto i";
                        type_track.push("i", "u0");
                    }
                    f << "=";
                    std::string step;
                    if (line.hasKey("step")) {
                        step = inlineCalc(_ms("step"), lang);
                    } else step = "1";
                    auto operFromStep = [](const std::string& step) -> std::string {
                        if (!step.empty() && step[0] == '-') return ">";
                        else return "<";
                    };
                    if (line.hasKey("begin")) {
                        f << _ms("begin") << ';';
                        if (line.hasKey("end")) {
                            f << var_name << operFromStep(step) << inlineCalc(_ms("end"), lang) << ";";
                        } else {
                            std::cerr << "LOOP no end for begin" << std::endl;
                            // TODO: error handling
                        }
                    } else if (line.hasKey("from")) {
                        f << inlineCalc(_ms("from"), lang) << ';';
                        if (line.hasKey("to")) {
                            f << var_name << operFromStep(step) << "=" << inlineCalc(_ms("to"), lang) << ";";
                        } else {
                            std::cerr << "LOOP no end for begin" << std::endl;
                            // TODO: error handling
                        }
                    }
                    if (step == "1") f << "++" << var_name;
                    else if (step == "-1") f << "--" << var_name;
                    else f << var_name << "+=" << step;
                    f << ") {";
                END_LANG
                ++indent_cnt;
                _contents_at_end.push("");
            CASE ("WHILE")
                Keys keys { "cond" };
                APPLY_KEYS("WHILE");
                LANG_CPP
                    f << "while (";
                    if (line.hasKey("cond")) {
                        Alg cond(inlineCalc(_ms("cond"), lang), macro_none, -1, -1, false, false, false);
                        cond.write(f, "cpp");
                    } else {
                        // TODO: handle error when no contents specified
                    }
                    f << ") {";
                END_LANG
                type_track++;
                ++indent_cnt;
                _contents_at_end.push("");
            // function is end
            CASE ("END")
                if (line.params().size() != 0) {
                    _warnings.push_back({"Parameters "});
                }
                if (lang != "cpp" && indent_cnt == 0) {
                    ERROR("More 'END' than should.");
                    continue;
                }
                f << _contents_at_end.top();
                _contents_at_end.pop();
                --indent_cnt;
                LANG_CPP f << "}\n";
                LANG_PY
                LANG_M f << INDENT << "end ";
                END_LANG
                try {
                    type_track--;
                } catch (const std::out_of_range& e) {
                    std::cerr << "ERROR: " << __FILE__ << " " << __LINE__
                              << ": " << e.what() << std::endl; 
                }
            DEFAULT
                // if (_add_semicolon) f << ";\n";
        END_SWITCH
        // clang-format on
        if (_add_comment) {
            if (func != "COMMENT") _wComment(f, lang, " ") << _raw_strings[i] << '\n';
        }
        if (i + 1 == _lines.size() && _branch_line != Alg::max_length && _alg_cnt < _macro.alg_num[_job_cnt]) {
            // meaning the last while BRANCH is not closed
            if (_alg_cnt + 1 < _macro.alg_num[_job_cnt]) { i = _branch_line - 1; }
            ++_alg_cnt;
            if (_recover_cnt == 0) {
                WARNING(fmt::format("No 'RECOVER' function found in estimation (Job: {}, Alg: {}).", _job_cnt + 1,
                                    _alg_cnt + 1));
                _log.war() << "No 'RECOVER' function found in estimation (Job: " << _job_cnt + 1
                           << ", Alg: " << _alg_cnt << ")." << std::endl;
            } else {
                _log.info() << "Found " << _recover_cnt << " 'RECOVER' function(s) in estimation (Job: " << _job_cnt + 1
                            << ", Alg: " << _alg_cnt << ")." << std::endl;
            }
            LANG_CPP
            if (_recover_cnt > 1) {
                f << "NMSE" << _job_cnt << "(ii, " << _alg_cnt - 1 << ") /= " << _recover_cnt << ";\n";
            }
            f << "}";
            END_LANG
            _recover_cnt = 0;
            try {
                type_track--;
            } catch (const std::out_of_range& e) {
                std::cerr << "ERROR: " << __FILE__ << " " << __LINE__ << ": " << e.what() << std::endl;
            }
        }
    }
    if (_errors.empty()) return true;
    else {
        _log.err() << "!! Encountering " << _errors.size() << " error(s) when processing ALG." << std::endl;
        Term::error("!! Encountering " + std::to_string(_errors.size()) + " errors when processing ALG.");
        for (auto&& e : _errors) {
            _log.err() << e.msg << " (line " << e.line << "): " << e.msg << std::endl;
            std::cerr << Term::ERR << "[ERROR] " << e.msg << Term::RESET << " (line " << Term::GREEN << e.line
                      << Term::RESET << ")\n"
                      << Term::ERR << "[ERROR] " << Term::RESET << "-> " << Term::YELLOW << e.raw_str << Term::RESET
                      << std::endl;
        }
        return false;
    }
}

#undef SWITCH_FUNC
#undef CASE
#undef END_SWITCH
#undef LANG_CPP
#undef LANG_PY
#undef LANG_M
#undef END_LANG
#undef ERROR
#undef WARNING
#undef INDENT
#undef _m
#undef _mi
#undef _ms
#undef APPLY_KEYS

std::string Alg::inlineCalc(const std::string& s, const std::string& lang) {
    // TODO: error handling here
    if (s.size() >= 2 && s[0] == '$' && *(s.end() - 1) == '$') {
        return Calc::as(s.substr(1, s.size() - 2), lang);
    } else if (s.size() >= 2 && s[0] == '"' && *(s.end() - 1) == '"') {
        return s.substr(1, s.size() - 2);
    } else {
        return Calc::as(s, lang);
    }
}

std::ofstream& Alg::_wComment(std::ofstream& f, const std::string& lang, const std::string& before) {
    f << before
      << ((lang == "cpp")                          ? "// "
          : (lang == "py" || lang == "ipynb")      ? "# "
          : (lang == "matlab" || lang == "octave") ? "% "
                                                   : "");
    return f;
}
