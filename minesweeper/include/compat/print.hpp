#ifndef LANGUAGE_CPP_PRINT_HPP
#define LANGUAGE_CPP_PRINT_HPP

#pragma once

// MinGW-w64 GCC (16.x) déclare __open_terminal et __write_to_terminal
// dans <print> mais ne les fournit pas dans libstdc++. On utilise donc
// un fallback std::format + std::cout sur cette plateforme.

#if defined(__MINGW32__) && defined(__GNUC__) && !defined(__clang__)

#  include <format>
#  include <iostream>
#  include <utility>

namespace compat
{
    template <typename... Args>
    void print(std::format_string<Args...> fmt, Args&&... args)
    {
        std::cout << std::format(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void println(std::format_string<Args...> fmt, Args&&... args)
    {
        std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
    }

    inline void println()
    {
        std::cout << '\n';
    }
}

#else

#  include <print>

namespace compat
{
    using std::print;
    using std::println;
}

#endif

#endif //LANGUAGE_CPP_PRINT_HPP
