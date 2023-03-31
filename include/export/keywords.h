/**
 * @file keywords.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Keywords List for C++, MATLAB and Python
 * @version 0.2.1
 * @date 2022-07-14
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#ifndef _EXPORT_KEYWORDS_H_
#define _EXPORT_KEYWORDS_H_

#include <array>
#include <string>

// Note that C++17 enables template argument deduction
// so the type and number can be omitted.

// C++ keywords
// Reference: https://github.com/jeff-hykin/better-cpp-syntax/blob/master/main/tokens.rb
static std::array CPP_Keywords = {
    // standard C++
    std::string("alignof"), std::string("alignas"), std::string("typeid"), std::string("noexcept"),
    std::string("noexcept"), std::string("static_cast"), std::string("dynamic_cast"), std::string("const_cast"),
    std::string("reinterpret_cast"), std::string("while"), std::string("for"), std::string("do"), std::string("if"),
    std::string("else"), std::string("goto"), std::string("switch"), std::string("try"), std::string("catch"),
    std::string("return"), std::string("break"), std::string("case"), std::string("continue"), std::string("default"),
    std::string("co_await"), std::string("co_yield"), std::string("co_return"), std::string("auto"),
    std::string("void"), std::string("char"), std::string("short"), std::string("int"), std::string("signed"),
    std::string("unsigned"), std::string("long"), std::string("float"), std::string("double"), std::string("bool"),
    std::string("wchar_t"), std::string("u_char"), std::string("u_short"), std::string("u_int"), std::string("u_long"),
    std::string("ushort"), std::string("uint"), std::string("u_quad_t"), std::string("quad_t"), std::string("qaddr_t"),
    std::string("caddr_t"), std::string("daddr_t"), std::string("div_t"), std::string("dev_t"), std::string("fixpt_t"),
    std::string("blkcnt_t"), std::string("blksize_t"), std::string("gid_t"), std::string("in_addr_t"),
    std::string("in_port_t"), std::string("ino_t"), std::string("key_t"), std::string("mode_t"), std::string("nlink_t"),
    std::string("id_t"), std::string("pid_t"), std::string("off_t"), std::string("segsz_t"), std::string("swblk_t"),
    std::string("uid_t"), std::string("id_t"), std::string("clock_t"), std::string("size_t"), std::string("ssize_t"),
    std::string("time_t"), std::string("useconds_t"), std::string("suseconds_t"), std::string("int8_t"),
    std::string("int16_t"), std::string("int32_t"), std::string("int64_t"), std::string("uint8_t"),
    std::string("uint16_t"), std::string("uint32_t"), std::string("uint64_t"), std::string("int_least8_t"),
    std::string("int_least16_t"), std::string("int_least32_t"), std::string("int_least64_t"),
    std::string("uint_least8_t"), std::string("uint_least16_t"), std::string("uint_least32_t"),
    std::string("uint_least64_t"), std::string("int_fast8_t"), std::string("int_fast16_t"), std::string("int_fast32_t"),
    std::string("int_fast64_t"), std::string("uint_fast8_t"), std::string("uint_fast16_t"),
    std::string("uint_fast32_t"), std::string("uint_fast64_t"), std::string("intptr_t"), std::string("uintptr_t"),
    std::string("intmax_t"), std::string("intmax_t"), std::string("uintmax_t"), std::string("uintmax_t"),
    std::string("uintmax_t"), std::string("NULL"), std::string("true"), std::string("false"), std::string("nullptr"),
    std::string("class"), std::string("struct"), std::string("union"), std::string("enum"), std::string("const"),
    std::string("static"), std::string("thread_local"), std::string("volatile"), std::string("register"),
    std::string("restrict"), std::string("extern"), std::string("inline"), std::string("constexpr"),
    std::string("consteval"), std::string("mutable"), std::string("friend"), std::string("explicit"),
    std::string("virtual"), std::string("final"), std::string("override"), std::string("volatile"),
    std::string("const"), std::string("noexcept"), std::string("constexpr"), std::string("mutable"),
    std::string("constexpr"), std::string("consteval"), std::string("private"), std::string("protected"),
    std::string("public"), std::string("if"), std::string("elif"), std::string("else"), std::string("endif"),
    std::string("ifdef"), std::string("ifndef"), std::string("define"), std::string("undef"), std::string("include"),
    std::string("line"), std::string("error"), std::string("warning"), std::string("pragma"), std::string("_Pragma"),
    std::string("defined"), std::string("__has_include"), std::string("__has_cpp_attribute"), std::string("this"),
    std::string("template"), std::string("namespace"), std::string("using"), std::string("operator"),
    std::string("typedef"), std::string("decltype"), std::string("constinit"), std::string("typename"),
    std::string("asm"), std::string("__asm__"), std::string("concept"), std::string("requires"), std::string("export"),
    std::string("atomic_cancel"), std::string("atomic_commit"), std::string("atomic_noexcept"), std::string("co_await"),
    std::string("co_return"), std::string("co_yield"), std::string("import"), std::string("module"),
    std::string("reflexpr"), std::string("synchronized"), std::string("audit"), std::string("axiom"),
    std::string("transaction_safe"), std::string("transaction_safe_dynamic"), std::string("cin"), std::string("wcin"),
    std::string("cout"), std::string("wcout"), std::string("cerr"), std::string("wcerr"), std::string("clog"),
    std::string("wclog"), std::string("stderr"), std::string("stdin"), std::string("stdout"), std::string("FILE"),
    std::string("fpos_t"), std::string("size_t"), std::string("pthread_t"), std::string("pthread_attr_t"),
    std::string("pthread_cond_t"), std::string("pthread_condattr_t"), std::string("pthread_mutex_t"),
    std::string("pthread_mutexattr_t"), std::string("pthread_once_t"), std::string("pthread_rwlock_t"),
    std::string("pthread_rwlockattr_t"), std::string("pthread_key_t"),
    // Armadillo
    std::string("armadillo"), std::string("arma")
};

// MATLAB / Octave keywords
// Use command "iskeyword"
static std::array MATLAB_Keywords = {
    // MATLAB specified keywords
    std::string("break"),     std::string("case"),     std::string("catch"),      std::string("classdef"),
    std::string("continue"),  std::string("else"),     std::string("elseif"),     std::string("end"),
    std::string("for"),       std::string("function"), std::string("global"),     std::string("if"),
    std::string("otherwise"), std::string("parfor"),   std::string("persistent"), std::string("return"),
    std::string("spmd"),      std::string("switch"),   std::string("try"),        std::string("while")
};

// Python keywords
// import keyword
// keyword.kwlist
static std::array PY_Keywords = {
    // Python keywords
    std::string("False"), std::string("None"), std::string("True"), std::string("__peg_parser__"), std::string("and"),
    std::string("as"), std::string("assert"), std::string("async"), std::string("await"), std::string("break"),
    std::string("class"), std::string("continue"), std::string("def"), std::string("del"), std::string("elif"),
    std::string("else"), std::string("except"), std::string("finally"), std::string("for"), std::string("from"),
    std::string("global"), std::string("if"), std::string("import"), std::string("in"), std::string("is"),
    std::string("lambda"), std::string("nonlocal"), std::string("not"), std::string("or"), std::string("pass"),
    std::string("raise"), std::string("return"), std::string("try"), std::string("while"), std::string("with"),
    std::string("yield"),
    // NumPy
    std::string("numpy"), std::string("np")
};

#endif
