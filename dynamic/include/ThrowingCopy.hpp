#ifndef LANGUAGE_CPP_THROWINGCOPY_H
#define LANGUAGE_CPP_THROWINGCOPY_H
#include <stdexcept>

struct ThrowingCopy {
    static int copy_count;
    static int throw_after;
    int value;

    explicit ThrowingCopy(const int v) : value(v) {
    }

    ThrowingCopy(const ThrowingCopy &other) : value(other.value) {
        if (++copy_count == throw_after)
            throw std::runtime_error("copy failed");
    }
};

#endif //LANGUAGE_CPP_THROWINGCOPY_H
