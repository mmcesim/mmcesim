/**
 * @file alg_line.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Alg_Line Class
 * @version 0.2.1
 * @date 2023-04-06
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "export/alg_line.h"
#include <iostream>

Alg_Line::Alg_Line(const std::string& str, ALG_Opt opt) : _raw_str(str), _opt(opt) {
    std::string s = str;
    _removeComment(s);
    if (s.empty()) return; // empty contents or all comments
    auto eq_index = _findChar(s, '=');
    _log.info() << ">> ALG Line: " << s << std::endl;
    // Now we need to tell whether this is assign '=' or parameter '='.
    bool eq_is_assign = false;
    std::string buf;
    if ((s.length() >= 2 && s.substr(0, 2) == "IF") ||
        (s.length() >= 3 && (s.substr(0, 3) == "FOR" || s.substr(0, 3) == "END" || s.substr(0, 3) == "CPP")) ||
        (s.length() >= 4 && (s.substr(0, 4) == "LOOP" || s.substr(0, 4) == "ELIF")) ||
        (s.length() >= 5 && s.substr(0, 5) == "WHILE") || (s.length() >= 7 && s.substr(0, 7) == "FOREVER")) {
        eq_is_assign = false;
    } else {
        if (eq_index != s.size()) {
            bool surely_not_assign = false;
            if (eq_index != 0 && (s[eq_index - 1] == '=' || s[eq_index - 1] == '!' || s[eq_index - 1] == '>' ||
                                  s[eq_index - 1] == '<')) {
                surely_not_assign = true;
            } else {
                if (eq_index + 1 != s.size()) {
                    if (isspace(s[eq_index + 1])) {
                        // Since parameter '=' should be strictly between key and val,
                        // like 'key=val', the space after '=' means that it should be the assign '='.
                        // If this does not satisfy, we cannot conclude it is parameter '='.
                        eq_is_assign = true;
                    }
                } else {
                    _log.err() << "Trailing '='." << std::endl;
                    throw std::runtime_error("Trailing '='.");
                }
            }
            if (!surely_not_assign && !eq_is_assign) {
                // Now that we have not concluded whether '=' is the assign '=',
                // we try to see the next word of '='.
                // If it is a function name (which should be in all capital),
                // we can conclude it is an assign '=' instead of parameter '='.
                std::stringstream ss(s.substr(eq_index + 1));
                if (ss >> buf) {
                    if (isFunc(buf)) eq_is_assign = true;
                    else eq_is_assign = false;
                } else {
                    _log.err() << "Trailing '='." << std::endl;
                    throw std::runtime_error("Trailing '='.");
                }
            }
        }
    }
    if (!eq_is_assign) {
        // If the equation we found is not assign '=',
        // we now pretend we have not found it at all!
        eq_index = s.size();
    }
    std::string s_returns     = "";
    std::string s_func_params = "";
    if (eq_index == s.size()) {
        // There is no valid '=', and therefore,
        // all contents are function name and parameters.
        s_func_params = s;
    } else {
        s_returns     = s.substr(0, eq_index);
        s_func_params = s.substr(eq_index + 1);
    }
    std::vector<std::string> unprocessed_returns;
    std::vector<std::string> unprocessed_func_params;
    std::stringstream ss_returns(s_returns);
    while (ss_returns >> buf) { unprocessed_returns.push_back(buf); }
    std::stack<char> st;
    buf.clear();
    for (size_t i = 0; i < s_func_params.length(); ++i) {
        char t = st.empty() ? 0 : st.top();
        char c = s_func_params[i];
        if (t == '\\') {
            st.pop();
            if (!st.empty() && st.top() == '"' || st.top() == '\'') {
                if (c == 'n') buf += '\n';
                else if (c == '"') buf += '"';
                else if (c == 't') buf += '\t';
                else if (c == 'r') buf += '\r';
                else if (c == '0') buf += '\0';
                else if (c == '$') buf += '$';
                else {
                    _log.err() << "Invalid escape." << std::endl;
                    throw std::runtime_error("Invalid escape.");
                }
            } else {
                // This is just an ALG function staring indication, not a real escape.
                buf += '\\';
                buf += c;
            }
        } else if (t == '"') {
            if (c == '\\') st.push('\\');
            else if (c == '"') {
                st.pop();
                buf += '"';
            } else buf += c;
        } else if (t == '$') {
            if (c == '\\') st.push('\\');
            else if (c == '$') {
                st.pop();
                buf += '$';
            } else buf += c;
        } else if (isspace(c)) {
            if (!buf.empty()) {
                unprocessed_func_params.push_back(buf);
                buf.clear();
            }
        } else if (c == '"') {
            st.push('"');
            buf += '"';
        } else if (c == '$') {
            st.push('$');
            buf += '$';
        } else buf += c;
    }
    if (!buf.empty()) unprocessed_func_params.push_back(buf);
    _processReturns(unprocessed_returns);
    _processFuncParams(unprocessed_func_params);
}

std::string::size_type Alg_Line::_findChars(const std::string& s, std::string cs) const noexcept {
    std::string::size_type iter = s.size();
    bool has_double_quote       = false;
    bool has_single_quote       = false;
    bool has_dollar             = false;
    bool is_escape              = false;
    for (auto&& e : s) {
        if (e == '\'' && !is_escape) has_single_quote = !has_single_quote;
        else if (e == '"' && !is_escape) has_double_quote = !has_double_quote;
        else if (e == '$' && !is_escape) has_dollar = !has_dollar;
        else if (contains(cs, e) && !is_escape && !has_single_quote && !has_double_quote && !has_dollar) {
            // Since C++11, std::string is contiguous.
            iter = &e - &s[0];
            break;
        } else if (e == '\\') is_escape = true;
        else is_escape = false;
    }
    return iter;
}

void Alg_Line::_removeComment(std::string& s) const noexcept { s.erase(s.begin() + _findChar(s, '#'), s.end()); }

void Alg_Line::_processReturns(const std::vector<std::string>& v) {
    for (auto&& s : v) {
        // '::' is indicator for type specification.
        auto type_loc = s.rfind("::");
        bool has_type = false;
        if (type_loc != std::string::npos) {
            has_type             = true;
            std::string type_str = s.substr(type_loc + 2);
            for (auto&& c : type_str) {
                if (c == '$' || c == '\'' || c == '"') {
                    // This is not a real type specification.
                    has_type = false;
                }
            }
        }
        Return_Type r;
        if (has_type) {
            r.name = s.substr(0, type_loc);
            r.type = s.substr(type_loc + 2);
        } else {
            r.name = s;
            r.type = "";
        }
        _returns.push_back(r);
    }
}

void Alg_Line::_processFuncParams(const std::vector<std::string>& v) {
    if (v.empty()) { throw std::runtime_error("No function name specified."); }
    std::vector<std::string>::size_type start_i = 0;
    if (auto&& func = v[0]; isFunc(func)) {
        _func   = func;
        start_i = 1;
    } else {
        // If no (correct) function name specified,
        // we assume it is 'CALC'.
        _func   = "CALC";
        start_i = 0;
    }
    if (_func == "CALC" || _func == "NEW" || _func == "RECOVER") {
        // do no more parsing for CALC or NEW
        Param_Type p;
        for (auto i = start_i; i != v.size(); ++i) { p.value += v[i]; }
        _log.info() << "CALC/NEW content: " << p.value << '\n';
        _params.push_back(p);
        return;
    } else if (_func == "WHILE" || _func == "CPP" || _func == "IF" || _func == "ELIF") {
        Param_Type p;
        for (auto i = start_i; i != v.size(); ++i) {
            p.value += v[i];
            // There needs space between tokens.
            if (_func == "CPP") p.value += ' ';
        }
        if (p.value.length() > 5 && p.value.substr(0, 5) == "cond=") { p.value = removeQuote(p.value.substr(5)); }
        _params.push_back(p);
        return;
    }
    for (auto i = start_i; i != v.size(); ++i) {
        auto&& s = v[i];
        // First find if it is the 'key=val' syntax.
        Param_Type p;
        auto eq_index               = _findChar(s, '=');
        size_t start_index_of_value = 0;
        if (eq_index == 0) {
            _log.err() << "Empty parameter key before '='." << std::endl;
            throw std::runtime_error("Empty parameter key before '='.");
        } else if (eq_index == s.size()) {
            // There is no '=' in the parameter
            p.key                = "";
            start_index_of_value = 0;
        } else {
            // The key is the text before '='.
            p.key                = s.substr(0, eq_index);
            start_index_of_value = eq_index + 1;
        }
        auto type_loc = s.rfind("::");
        bool has_type = false;
        if (type_loc != std::string::npos) {
            has_type             = true;
            std::string type_str = s.substr(type_loc + 2);
            for (auto&& c : type_str) {
                if (c == '$' || c == '\'' || c == '"') {
                    // This is not a real type specification.
                    has_type = false;
                }
            }
        }
        if (has_type) {
            p.value = s.substr(start_index_of_value, type_loc);
            p.type  = s.substr(type_loc + 2);
        } else {
            p.value = s.substr(start_index_of_value);
            p.type  = "";
        }
        _params.push_back(p);
    }
}
