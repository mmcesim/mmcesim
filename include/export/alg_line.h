/**
 * @file alg_line.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Parse Line of Alg
 * @version 0.1.0
 * @date 2022-07-22
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _ALG_LINE_H_
#define _ALG_LINE_H_

#include <string>
#include <vector>
#include <any>

class Alg_Line {
public:
    struct Return_Type {
        std::string name;
        std::string type;
    };

    struct Param_Type {
        std::string name;
        std::string type;
        std::string key;
        std::any value;
    };

public:
    Alg_Line() = default;

    Alg_Line(const std::string& str);

    const std::vector<Return_Type>& returns() const;

    const Return_Type& returns(std::vector<Return_Type>::size_type index) const;

    const std::vector<Param_Type>& params() const;

    const Param_Type& params(std::vector<Param_Type>::size_type index) const;

private:
    std::vector<Return_Type> _returns;
    std::vector<Param_Type> _params;
};

#endif
