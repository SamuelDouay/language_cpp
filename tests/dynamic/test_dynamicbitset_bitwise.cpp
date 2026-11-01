#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("operator~ flips all bits", "[bitset]") {
    DynamicBitset bs("10110");
    DynamicBitset result = ~bs;

    REQUIRE(result.size() == 5);
    REQUIRE(result.to_string() == "01001");
}

TEST_CASE("operator& applies AND", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b("11010");

    DynamicBitset r = a & b;

    REQUIRE(r.to_string() == "10010");
}

TEST_CASE("operator| applies OR", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b("11010");

    DynamicBitset r = a | b;

    REQUIRE(r.to_string() == "11110");
}

TEST_CASE("operator^ applies XOR", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b("11010");

    DynamicBitset r = a ^ b;

    REQUIRE(r.to_string() == "01100");
}

TEST_CASE("operator&= modifies in place", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b("11010");

    a &= b;

    REQUIRE(a.to_string() == "10010");
}

TEST_CASE("operator|= modifies in place", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b("11010");

    a |= b;

    REQUIRE(a.to_string() == "11110");
}

TEST_CASE("operator^= modifies in place", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b("11010");

    a ^= b;

    REQUIRE(a.to_string() == "01100");
}

TEST_CASE("bitwise operators throw on size mismatch", "[bitset]") {
    DynamicBitset a("101");
    DynamicBitset b("1010");

    REQUIRE_THROWS_AS(a & b, std::invalid_argument);
    REQUIRE_THROWS_AS(a | b, std::invalid_argument);
    REQUIRE_THROWS_AS(a ^ b, std::invalid_argument);
    REQUIRE_THROWS_AS(a &= b, std::invalid_argument);
}
