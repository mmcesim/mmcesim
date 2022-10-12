/**
 * @file simulate.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Simulate Generated Code
 * @version 0.1.0
 * @date 2022-10-12
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _SIMULATE_H_
#define _SIMULATE_H_

#include <iostream>
#include <string>
#pragma GCC diagnostic ignored "-Wc++11-narrowing"
#pragma GCC diagnostic push
#include <boost/process.hpp>
#pragma GCC diagnostic pop
#include "export/shared_info.h"
#include "utils.h"

class Simulate {
public:
    Simulate(const Shared_Info& info);

    int simulate() const;

    static int simulate(const Shared_Info& info);

private:
    Shared_Info _s_info;
};

#endif
