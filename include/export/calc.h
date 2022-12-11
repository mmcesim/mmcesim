/**
 * @file calc.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Make Calculation in Alg
 * @version 0.1.0
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_CALC_H_
#define _EXPORT_CALC_H_

#include "export/type_track_global.h"
#include "utils.h"
#include <boost/algorithm/string/replace.hpp>
#include <string>
#include <tuple>

class Calc {
  public:
    Calc(const std::string& str);

    /**
     * @brief Convert the Alg CALC contents into a language command.
     *
     * @param lang Language ("cpp", "py", "matlab" or "octave").
     * @param msg Pointer to error message string.
     * @return (std::string) The converted string.
     */
    std::string as(std::string lang, std::string* msg = nullptr) const;

    static std::string as(const std::string& str, std::string lang, std::string* msg = nullptr);

  private:
    bool _changeOperator(std::string& str, std::string lang, std::string* msg = nullptr) const;

    bool _changeFunction(std::string& str, std::string lang, std::string* msg = nullptr) const;

    bool _changeSuperScript(std::string& str, std::string lang, std::string* msg = nullptr) const;

    bool _changeSubScript(std::string& str, std::string lang, int8_t* d = nullptr, std::string* msg = nullptr) const;

    bool _changeParen(std::string& str, std::string lang, std::string* msg = nullptr) const;

    void _msg(std::string* msg, const std::string& content) const;

    void _addArma(std::string& str, const std::string& func) const;

    void _addMmce(std::string& str, const std::string& func) const;

    std::string _str;
};

inline std::string Calc::as(const std::string& str, std::string lang, std::string* msg) {
    Calc calc(str);
    return calc.as(lang);
}

inline void Calc::_msg(std::string* msg, const std::string& content) const {
    if (msg) *msg = content;
}

inline void Calc::_addArma(std::string& str, const std::string& func) const {
    boost::replace_all(str, std::string("\\") + func, "arma::" + func);
}

inline void Calc::_addMmce(std::string& str, const std::string& func) const {
    boost::replace_all(str, std::string("\\") + func, "mmce::" + func);
}

#endif
