#include <catch2/catch_test_macros.hpp>
#include "MiniVector.hpp"
#include <stdexcept>

TEST_CASE("at throws out_of_range on invalid index", "[minivector][access]") {
    MiniVector<int> v = {10, 20, 30};
    REQUIRE_THROWS_AS(v.at(3), std::out_of_range);
    REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    REQUIRE_NOTHROW(v.at(0));
}

TEST_CASE("front returns first element", "[minivector][access]") {
    MiniVector<int> v = {1, 2, 3};
    REQUIRE(v.front() == 1);
    v.front() = 10;
    REQUIRE(v[0] == 10);
}

TEST_CASE("front throws on empty vector", "[minivector][access]") {
    MiniVector<int> v;
    REQUIRE_THROWS_AS(v.front(), std::out_of_range);
}

TEST_CASE("back returns last element", "[minivector][access]") {
    MiniVector<int> v = {1, 2, 3};
    REQUIRE(v.back() == 3);
    v.back() = 30;
    REQUIRE(v[2] == 30);
}

TEST_CASE("back throws on empty vector", "[minivector][access]") {
    MiniVector<int> v;
    REQUIRE_THROWS_AS(v.back(), std::out_of_range);
}

TEST_CASE("front/back const versions work", "[minivector][access]") {
    const MiniVector<int> v = {1, 2, 3};
    REQUIRE(v.front() == 1);
    REQUIRE(v.back() == 3);
}

TEST_CASE("data returns pointer to underlying array", "[minivector][access]") {
    MiniVector<int> v = {1, 2, 3};
    int *ptr = v.data();
    REQUIRE(ptr != nullptr);
    REQUIRE(ptr[0] == 1);
    REQUIRE(ptr[2] == 3);
}

TEST_CASE("data returns nullptr for empty vector", "[minivector][access]") {
    MiniVector<int> v;
    REQUIRE(v.data() == nullptr);
}

TEST_CASE("data const version works", "[minivector][access]") {
    const MiniVector<int> v = {1, 2, 3};
    const int *ptr = v.data();
    REQUIRE(ptr != nullptr);
    REQUIRE(ptr[1] == 2);
}

TEST_CASE("const operator[] and at work", "[minivector][access]") {
    const MiniVector<int> v = {10, 20, 30};
    REQUIRE(v[0] == 10);
    REQUIRE(v.at(1) == 20);
    REQUIRE_THROWS_AS(v.at(3), std::out_of_range);
}
