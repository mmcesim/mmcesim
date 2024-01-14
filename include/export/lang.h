/**
 * @file lang.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Export Languages
 * @version 0.2.1
 * @date 2023-05-04
 *
 * @copyright Copyright (c) 2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_LANG_H_
#define _EXPORT_LANG_H_

/**
 * @brief Export language.
 *
 * Options are CPP, MATLAB, PYTHON and IPYNB.
 */
enum class Lang {
    CPP,    ///< C++ with Armadillo library
    MATLAB, ///< MATLAB
    OCTAVE, ///< GNU Octave
    PY,     ///< Python with NumPy library
    IPYNB   ///< IPyNb with NumPy library
};

#endif
