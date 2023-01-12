/**
 * @file term.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Term Class and Color/Style Definitions
 * @version 0.1.1
 * @date 2023-01-12
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "term.h"

void Term::disable() { Term::BLACK.vld = false; }

void Term::enable() { Term::BLACK.vld = true; }
