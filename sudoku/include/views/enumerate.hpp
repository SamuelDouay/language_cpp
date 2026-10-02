#ifndef LANGUAGE_CPP_ENUMERATE_HPP
#define LANGUAGE_CPP_ENUMERATE_HPP

#pragma once

#include <cstddef>
#include <ranges>
#include <utility>

#if __cpp_lib_ranges_enumerate >= 202302L
#  include <ranges>

namespace views
{
    using std::views::enumerate;
}
#else

namespace views
{
    struct enumerate_fn
        : std::ranges::range_adaptor_closure<enumerate_fn>
    {
        template <std::ranges::viewable_range R>
        auto enumerate(R&& r)
        {
            return std::views::zip(
                std::views::iota(std::size_t{0}),
                std::forward<R>(r)
            );
        }
    }

    inline constexpr enumerate_fn enumerate;
}
#endif
#endif //LANGUAGE_CPP_ENUMERATE_HPP
