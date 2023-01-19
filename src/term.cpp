/**
 * @file term.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Term Class and Color/Style Definitions
 * @version 0.2.0
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "term.h"

void Term::disable() {
    Term::BLACK.vld      = false;
    Term::RED.vld        = false;
    Term::GREEN.vld      = false;
    Term::YELLOW.vld     = false;
    Term::BLUE.vld       = false;
    Term::MAGENTA.vld    = false;
    Term::CYAN.vld       = false;
    Term::WHITE.vld      = false;
    Term::DEFAULT.vld    = false;
    Term::BG_BLACK.vld   = false;
    Term::BG_RED.vld     = false;
    Term::BG_GREEN.vld   = false;
    Term::BG_YELLOW.vld  = false;
    Term::BG_BLUE.vld    = false;
    Term::BG_MAGENTA.vld = false;
    Term::BG_CYAN.vld    = false;
    Term::BG_WHITE.vld   = false;
    Term::BG_DEFAULT.vld = false;
    Term::RESET.vld      = false;
    Term::BF.vld         = false;
    Term::DIM.vld        = false;
    Term::IT.vld         = false;
    Term::UL.vld         = false;
    Term::BLINK.vld      = false;
    Term::INV.vld        = false;
    Term::OFF_BF.vld     = false;
    Term::OFF_DIM.vld    = false;
    Term::OFF_IT.vld     = false;
    Term::OFF_UL.vld     = false;
    Term::OFF_BLINK.vld  = false;
    Term::OFF_INV.vld    = false;
    Term::ERR.vld        = false;
    Term::MSG.vld        = false;
}

void Term::enable() {
    Term::BLACK.vld      = true;
    Term::RED.vld        = true;
    Term::GREEN.vld      = true;
    Term::YELLOW.vld     = true;
    Term::BLUE.vld       = true;
    Term::MAGENTA.vld    = true;
    Term::CYAN.vld       = true;
    Term::WHITE.vld      = true;
    Term::DEFAULT.vld    = true;
    Term::BG_BLACK.vld   = true;
    Term::BG_RED.vld     = true;
    Term::BG_GREEN.vld   = true;
    Term::BG_YELLOW.vld  = true;
    Term::BG_BLUE.vld    = true;
    Term::BG_MAGENTA.vld = true;
    Term::BG_CYAN.vld    = true;
    Term::BG_WHITE.vld   = true;
    Term::BG_DEFAULT.vld = true;
    Term::RESET.vld      = true;
    Term::BF.vld         = true;
    Term::DIM.vld        = true;
    Term::IT.vld         = true;
    Term::UL.vld         = true;
    Term::BLINK.vld      = true;
    Term::INV.vld        = true;
    Term::OFF_BF.vld     = true;
    Term::OFF_DIM.vld    = true;
    Term::OFF_IT.vld     = true;
    Term::OFF_UL.vld     = true;
    Term::OFF_BLINK.vld  = true;
    Term::OFF_INV.vld    = true;
    Term::ERR.vld        = true;
    Term::MSG.vld        = true;
}
