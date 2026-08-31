#include <stdexcept>

#ifndef LANGUAGE_CPP_THROWING_H
#define LANGUAGE_CPP_THROWING_H

struct Throwing
{
    static int constructions;
    static int throw_after;

    int value;

    explicit Throwing(int v)
        : value(v)
    {
        ++constructions;

        if (constructions > throw_after)
            throw std::runtime_error("construction failed");
    }

    Throwing(const Throwing&) = delete;
    Throwing& operator=(const Throwing&) = delete;

    Throwing(Throwing&& other) noexcept
        : value(other.value)
    {
    }

    Throwing& operator=(Throwing&&) = default;
};

#endif //LANGUAGE_CPP_THROWING_H
