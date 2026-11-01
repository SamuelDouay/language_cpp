#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("operator<< shifts bits left", "[bitset]") {
    DynamicBitset bs("00010110");

    bs <<= 2;

    REQUIRE(bs.to_string() == "01011000");
}

TEST_CASE("operator>> shifts bits right", "[bitset]") {
    DynamicBitset bs("10110000");

    bs >>= 3;

    REQUIRE(bs.to_string() == "00010110");
}

TEST_CASE("shift by zero does nothing", "[bitset]") {
    DynamicBitset bs("10110");

    bs <<= 0;

    REQUIRE(bs.to_string() == "10110");
}

TEST_CASE("shift by size or more clears all bits", "[bitset]") {
    DynamicBitset bs("10110");

    bs <<= 10;

    REQUIRE(bs.none());
}

TEST_CASE("free operator<< does not modify source", "[bitset]") {
    DynamicBitset bs("00010110");

    DynamicBitset r = bs << 2;

    REQUIRE(bs.to_string() == "00010110");
    REQUIRE(r.to_string() == "01011000");
}
