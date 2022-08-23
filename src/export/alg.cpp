/**
 * @file alg.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of class Alg
 * @version 0.1.0
 * @date 2022-07-25
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include "export/alg.h"

Alg::Alg(const std::string& str, bool fail_fast, bool add_comment, bool add_semicolon)
    : _failed(false), _add_semicolon(add_semicolon), _add_comment(add_comment) {
    std::stringstream ss(str);
    std::string line;
    std::string unterminated_line = "";
    Alg_Lines::size_type line_no = 0;
    while (std::getline(ss, line, '\n')) { // separate by newline
        ++line_no;
        trim(line); // remove the whitespace around it.
        if (line.empty()) continue; // skip empty line
        // bool unterminated_here = *(line.end() - 1) == '\\';
        if (*(line.end() - 1) == '\\') {
            // The last character is backslash
            // so the line has not terminated.
            unterminated_line += std::string(" ") + line;
        } else {
            auto s = unterminated_line + line;
            try {
                Alg_Line l(s);
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
                _errors.push_back({e.what(), s, line_no});
            }
            unterminated_line.clear();
        }
    }
}

#define SWITCH_FUNC if (false) {
#define CASE(_func_name__) } else if (func == _func_name__) {
#define END_SWITCH }
#define LANG_CPP if (lang == "cpp") {
#define LANG_PY } else if (lang == "py") {
#define LANG_M } else if (lang == "matlab" || lang == "octave") {
#define END_LANG } else throw("Unknown language " + lang + ".");
#define ERROR(msg) _errors.push_back({msg, _raw_strings[i], _line_nos[i]})
#define WARNING(msg) _warnings.push_back({msg, _raw_strings[i], _line_nos[i]})
#define INDENT _indent(_indent_size)

// Apply keys and check repeated/unknown keys.
#define APPLY_KEYS(_func_name__) \
    if (!_applyKey(line, keys)) \
        ERROR(std::string("More parameters than expected in '") + _func_name__ + "'."); \
    if (line.hasRepeatedKey()) \
        ERROR(std::string("Repeated key in '") + _func_name__ + "'."); \
    if (line.hasUnknownKey(keys)) \
        ERROR(std::string("Unkown key in '") + _func_name__ + "'.");

bool Alg::write(std::ofstream& f, const std::string& lang) {
    size_t indent_cnt = 0; // used for Python and MATLAB.
    for (decltype(_lines.size()) i = 0; i != _lines.size(); ++i) {
        Alg_Line line = _lines[i];
        const std::string& func = line.func();
        std::cout << "func: '" << func << "'\n";
        SWITCH_FUNC
            // function no end
            CASE ("BREAK")
                f << "break";
                LANG_CPP
                    if (_add_semicolon) f << ";";
                END_LANG
            CASE ("CALC")
                std::string msg;
                std::string out;
                LANG_CPP
                    if (line.params().size() == 0) {
                        if (_add_semicolon) f << ";";
                    } else {
                        out = Calc::as(line.params(0).value, "cpp", &msg);
                        if (msg.empty()) {
                            if (size_t s = line.returns().size(); s != 0) {
                                if (s == 1) {
                                    f << line.returns(0).name;
                                } else {
                                    // TODO: multiple return values
                                }
                                f << "=";
                            }
                            f << out;
                            if (_add_semicolon) f << ";";
                        } else {
                            std::cerr << msg << "\n";
                            // TODO: handle error here
                        }
                    }
                LANG_PY
                LANG_M
                END_LANG
            CASE ("COMMENT")
                std::string comment;
                if (line.params().size() > 1) {
                    for (auto&& s : line.params()) comment += s.value + ' ';
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
                        cpp_content = line.params(0).value;
                        f << cpp_content;
                    }
                    if (!cpp_content.empty() && *(cpp_content.cend() - 1) != ';' && _add_semicolon) {
                        f << ';';
                    }
                END_LANG
            CASE ("INIT")
                std::cout << "I am in INIT" << std::endl;
                for (auto&& param : line.params()) {
                    std::cout << param.value << '\n';
                }
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
                    auto cppScaleFill = [&line, &f] (const Type& t) -> std::string {
                        f << t.string() << " " << line.returns(0).name << " = ";
                        if (t.dim() > 0) {
                            if (line.hasKey("scale")) {
                                f << '(' << removeQuote(line["scale"]) << ") * ";
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
                                    if (line.hasKey("scale")) fill = removeQuote(line["scale"]);
                                    else fill = "1";
                                } else {
                                    f << '(' << removeQuote(line["scale"]) << ") * ";
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
                                    f << "arma::" << fill << '(' << line["dim1"] << ", "
                                        << line["dim2"] << ", " << line["dim3"] << ")";
                                END_LANG
                            } else {
                                // dim: 2 (a matrix)
                                Type type = getReturnType('2');
                                LANG_CPP
                                    auto fill = cppScaleFill(type);
                                    f << "arma::" << fill << '(' << line["dim1"] << ", " << line["dim2"] << ")";
                                END_LANG
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
                                    f << "arma::" << fill << "(1, " << line["dim1"] << ")";
                                } else if (Type type = s; type.dim() == 0) {
                                    // scalar assigning can just use the 
                                    if (line.hasKey("scale")) {
                                        f << removeQuote(line["scale"]) << "";
                                    } else {
                                        f << removeQuote(line["dim1"]) << "";
                                    }
                                } else {
                                    f << "arma::" << fill << '(' << line["dim1"] << ")";
                                }
                            END_LANG
                        }
                    } else {
                        // dim: 0 (a scalar)
                        Type type = getReturnType('0');
                        LANG_CPP
                            auto fill = cppScaleFill(type);
                            f << fill << "()"; // this may not be correct since it is such a long time
                        END_LANG
                    }
                    LANG_CPP
                        if (_add_semicolon) f << ";";
                    END_LANG
                }
            CASE ("MATLAB")
            CASE ("OCTAVE")
            CASE ("PRINT")
            // function needs end
            CASE ("ELSE")
                LANG_CPP
                    f << "} else {";
                END_LANG
            CASE ("ELIF")
                Keys keys { "cond" };
                APPLY_KEYS("ELIF");
                LANG_CPP
                    f << "} else if (";
                    if (line.hasKey("cond")) {
                        Alg cond(removeQuote(line["cond"]), false, false, false);
                        cond.write(f, "cpp");
                    } else {
                        // TODO: handle error when no contents specified
                    }
                    f << ") {";
                END_LANG
            CASE ("FOR")
                Keys keys { "init", "cond", "oper" };
                APPLY_KEYS("FOR");
                // init call INIT/CALC function
                LANG_CPP
                    f << "for (";
                    std::cout << "line[init] = " << line["init"] << '\n';
                    if (line.hasKey("init")) {
                        Alg init(removeQuote(line["init"]), false, false, true);
                        init.write(f, "cpp");
                    } else f << ";";
                    if (line.hasKey("cond")) {
                        Alg cond(removeQuote(line["cond"]), false, false, true);
                        cond.write(f, "cpp");
                    } else f << ";";
                    if (line.hasKey("oper")) {
                        Alg oper(removeQuote(line["oper"]), false, false, false);
                        oper.write(f, "cpp");
                        std::cout << "has oper!\n";
                    }
                    f << ") {";
                END_LANG
                ++indent_cnt;
            CASE ("FOREVER")
                LANG_CPP
                    f << "while(1) {";
                END_LANG
            CASE ("IF")
                Keys keys { "cond" };
                APPLY_KEYS("IF");
                LANG_CPP
                    f << "if (";
                    if (line.hasKey("cond")) {
                        Alg cond(removeQuote(line["cond"]), false, false, false);
                        cond.write(f, "cpp");
                    } else {
                        // TODO: handle error when no contents specified
                    }
                    f << ") {";
                END_LANG
                ++indent_cnt;
            CASE ("LOOP")
                Keys keys { "begin", "end", "step", "from", "to" };
                APPLY_KEYS("LOOP");
                LANG_CPP
                    std::string var_name = "i";
                    f << "for (";
                    if (!line.returns().empty()) {
                        if (auto&& s = line.returns(0).type; !s.empty()) {
                            Type iter_type = s;
                            f << iter_type.string() << ' ';
                        } else {
                            f << "auto ";
                        }
                        var_name = line.returns(0).name;
                        f << var_name;
                    } else {
                        f << "auto i";
                    }
                    f << "=";
                    std::string step;
                    if (line.hasKey("step")) {
                        step = line["step"];
                    } else step = "1";
                    auto operFromStep = [](const std::string& step) -> std::string {
                        if (!step.empty() && step[0] == '-') return ">";
                        else return "<";
                    };
                    if (line.hasKey("begin")) {
                        f << line["begin"] << ';';
                        if (line.hasKey("end")) {
                            f << var_name << operFromStep(step) << line["end"] << ";";
                        } else {
                            std::cerr << "LOOP no end for begin" << std::endl;
                            // TODO: error handling
                        }
                    } else if (line.hasKey("from")) {
                        f << line["from"] << ';';
                        if (line.hasKey("to")) {
                            f << var_name << operFromStep(step) << "=" << line["to"] << ";";
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
            CASE ("WHILE")
                Keys keys { "cond" };
                APPLY_KEYS("WHILE");
                LANG_CPP
                    f << "while (";
                    if (line.hasKey("cond")) {
                        Alg cond(removeQuote(line["cond"]), false, false, false);
                        cond.write(f, "cpp");
                    } else {
                        // TODO: handle error when no contents specified
                    }
                    f << ") {";
                END_LANG
                ++indent_cnt;
            // function is end
            CASE ("END")
                if (line.params().size() != 0) {
                    _warnings.push_back({"Parameters "});
                }
                if (lang != "cpp" && indent_cnt == 0) {
                    ERROR("More 'END' than should.");
                    continue;
                }
                --indent_cnt;
                LANG_CPP f << "}";
                LANG_PY
                LANG_M f << INDENT << "end";
                END_LANG
        END_SWITCH
        if (_add_comment) {
            if (func != "COMMENT") _wComment(f, lang, " ") << _raw_strings[i] << '\n';
        }
    }
    return true;
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
#undef APPLY_KEYS

std::ofstream& Alg::_wComment(std::ofstream& f, const std::string& lang, const std::string& before) {
    f << before 
      << ((lang == "cpp") ? "// " :
          (lang == "py" || lang == "ipynb") ? "# " :
          (lang == "matlab" || lang == "octave") ? "% " : "");
    return f;
}
