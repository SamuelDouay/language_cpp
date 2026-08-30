#include <catch2/catch_test_macros.hpp>
#include "MiniVector.hpp"

TEST_CASE("iterators work with range-based for", "[minivector][iterator]") {
    MiniVector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int x: v)
        sum += x;
    REQUIRE(sum == 15);
}

TEST_CASE("begin/end on empty vector return same pointer", "[minivector][iterator]") {
    MiniVector<int> v;
    REQUIRE(v.begin() == v.end());
    REQUIRE(v.begin() == nullptr);
}

TEST_CASE("const begin/end work", "[minivector][iterator]") {
    const MiniVector<int> v = {1, 2, 3};
    const int *b = v.begin();
    const int *e = v.end();
    REQUIRE(e - b == 3);
    REQUIRE(b[0] == 1);
    REQUIRE(b[2] == 3);
}
