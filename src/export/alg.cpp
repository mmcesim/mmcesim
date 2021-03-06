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

Alg::Alg(const std::string& str, bool fail_fast) : _failed(false) {
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

#define SWITCH_FUNC if (true) {
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
        SWITCH_FUNC
            // function no end
            CASE ("CALC")
                LANG_CPP
                LANG_PY
                LANG_M
                END_LANG
            CASE ("COMMENT")
                std::string comment;
                if (line.params().size() > 1) {
                    comment = line.params(0).value;
                    for (auto&& s : line.params()) comment += s.value + ' ';
                } else if (line.params().empty()) WARNING("Empty comment content.");
                else comment = line.params(0).value;
                trim(comment);
                if (isQuoted(comment))
                    _wComment(f, lang, INDENT) << std::quoted(comment);
                else _wComment(f, lang, INDENT) << comment;
            CASE ("INIT")
                if (line.returns().size() > 1) ERROR("Return variable more than 1 in 'INIT'.");
                else if (line.returns().empty()) WARNING("Unused 'INIT', i.e. no return variable.");
                else {
                    Keys keys { "dim1", "dim2", "dim3", "fill", "scale", "dtype" };
                    APPLY_KEYS("INIT");
                    auto getReturnType = [&line] (char dim) {
                        if (auto&& s = line.returns(0).type; s.empty()) {
                            // 'dtype' is ignored if the return type is specified
                            return s;
                        } else {
                            return (line.hasKey("dtype") ?
                                    removeQuote(line["dtype"]) :
                                    std::string("c")) + dim;
                        }
                    };
                    auto cppScaleFill = [&line, &f] (const Type& t) {
                        f << t.string() << " " << line.returns(0).name << " = ";
                        if (line.hasKey("scale")) {
                            f << '(' << line["scale"] << ") * ";
                        }
                        std::string fill = "zeros";
                        if (line.hasKey("fill")) {
                            fill = boost::algorithm::to_lower_copy(line["fill"]);
                        }
                        return fill;
                    };
                    if (line.hasKey("dim1")) {
                        if (line.hasKey("dim2")) {
                            if (line.hasKey("dim3")) {
                                // dim: 3 (a tensor)
                                Type type = getReturnType('3');
                                LANG_CPP
                                    auto fill = cppScaleFill(type);
                                    f << "arma::" << fill << '(' << line["dim1"] << ", "
                                      << line["dim2"] << ", " << line["dim3"] << ");";
                                END_LANG
                            } else {
                                // dim: 2 (a matrix)
                                Type type = getReturnType('2');
                                LANG_CPP
                                    auto fill = cppScaleFill(type);
                                    f << "arma::" << fill << '(' << line["dim1"] << ", " << line["dim2"] << ");";
                                END_LANG
                            }
                        } else {
                            // dim: 1 (a vector)
                            Type type = getReturnType('1');
                            LANG_CPP
                                auto fill = cppScaleFill(type);
                                f << "arma::" << fill << '(' << line["dim1"] << ");";
                            END_LANG
                        }
                    } else {
                        // dim: 0 (a scalar)
                        Type type = getReturnType('0');
                        LANG_CPP
                            auto fill = cppScaleFill(type);
                            // TODO: scalar initialization
                        END_LANG
                    }
                }
            CASE ("PRINT")
            // function needs end
            CASE ("FOR")
            CASE ("FOREVER")
            CASE ("LOOP")
            CASE ("WHILE")
            // function is end
            CASE ("END")
                if (line.params().size() != 0) {
                    _warnings.push_back({"Parameters "});
                }
                if (indent_cnt == 0) {
                    ERROR("More 'END' than should.");
                    continue;
                }
                --indent_cnt;
                LANG_CPP f << "}";
                LANG_PY
                LANG_M f << INDENT << "end";
                END_LANG
        END_SWITCH
        _wComment(f, lang, " ") << _raw_strings[i] << '\n';
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
