/**
 * @file fmt.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Extending {fmt}
 * @version 0.2.1
 * @date 2023-05-04
 *
 * @copyright Copyright (c) 2023 Wuqiong Zhao (Teddy van Jerry)
 *
 * @details `std::any` now can be used in {fmt} format string.
 *
 */

#ifndef _FMT_H_
#define _FMT_H_

#include <any>
#include <fmt/core.h>
#include <fmt/format.h>

template <>
struct fmt::formatter<std::any> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        // No format string is needed, so just return the end iterator.
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const std::any& value, FormatContext& ctx) -> decltype(ctx.out()) {
        // Use std::any_cast to extract the value from the std::any object.
        if (const auto* x = std::any_cast<const char>(&value)) return fmt::format_to(ctx.out(), "{}", *x);
        else if (const auto* x = std::any_cast<const int>(&value)) return fmt::format_to(ctx.out(), "{}", *x);
        else if (const auto* x = std::any_cast<const unsigned>(&value)) return fmt::format_to(ctx.out(), "{}", *x);
        else if (const auto* x = std::any_cast<const size_t>(&value)) return fmt::format_to(ctx.out(), "{}", *x);
        else if (const auto* x = std::any_cast<const std::string>(&value)) return fmt::format_to(ctx.out(), "{}", *x);
        else if (const auto* x = std::any_cast<const double>(&value)) return fmt::format_to(ctx.out(), "{}", *x);
        else {
            assert(false && "Unknown type in std::any fmt!");
            return fmt::format_to(ctx.out(), "<unknown type>");
        }
    }
};

#endif
