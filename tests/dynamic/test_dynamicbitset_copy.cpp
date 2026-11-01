#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("copy preserves state", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b = a;

    REQUIRE(a == b);

    b.flip(0);

    REQUIRE_FALSE(a == b);
}

TEST_CASE("assignment copies state", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b("11");

    b = a;

    REQUIRE(b == a);
    REQUIRE(b.size() == 5);
}
