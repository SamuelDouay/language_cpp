#include <catch2/catch_test_macros.hpp>
#include "MiniVector.hpp"

TEST_CASE("MiniVector default constructor", "[minivector][constructor]") {
    MiniVector<int> v;
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}

TEST_CASE("MiniVector constructor with count and value", "[minivector][constructor]") {
    MiniVector<int> v(5, 42);
    REQUIRE_FALSE(v.empty());
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    for (std::size_t i = 0; i < v.size(); ++i)
        REQUIRE(v[i] == 42);
}

TEST_CASE("MiniVector initializer_list constructor", "[minivector][constructor]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};
    REQUIRE(v.size() == 5);
    REQUIRE(v[0] == 1);
    REQUIRE(v[4] == 5);
}

TEST_CASE("copy constructor deep copies", "[minivector][constructor]") {
    MiniVector<int> original = {1, 2, 3};
    MiniVector<int> copy(original);
    REQUIRE(copy.size() == original.size());
    copy[0] = 99;
    REQUIRE(original[0] == 1);
}

TEST_CASE("assignment operator deep copies", "[minivector][constructor]") {
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b;
    b = a;
    REQUIRE(b.size() == a.size());
    b[0] = -1;
    REQUIRE(a[0] == 1);
}

TEST_CASE("move constructor leaves source empty", "[minivector][constructor]") {
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b(std::move(a));
    REQUIRE(b.size() == 3);
    REQUIRE(a.size() == 0);
    REQUIRE(a.capacity() == 0);
}

TEST_CASE("move assignment leaves source empty", "[minivector][constructor]") {
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b;
    b = std::move(a);
    REQUIRE(b.size() == 3);
    REQUIRE(a.size() == 0);
    REQUIRE(a.capacity() == 0);
}

TEST_CASE("assignment operator releases old memory", "[minivector][constructor]") {
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {4, 5, 6, 7};
    b = a;
    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 1);
}

TEST_CASE("move assignment releases old memory", "[minivector][constructor]") {
    MiniVector<int> a = {1, 2, 3};
    MiniVector<int> b = {4, 5, 6, 7};
    b = std::move(a);
    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == 1);
    REQUIRE(a.size() == 0);
}
