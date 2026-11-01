#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("operator== returns true for identical bitsets", "[bitset]") {
    DynamicBitset a("10110");
    DynamicBitset b("10110");

    REQUIRE(a == b);
    REQUIRE_FALSE(a != b);
}

TEST_CASE("operator== returns false for different sizes", "[bitset]") {
    DynamicBitset a("101");
    DynamicBitset b("1010");

    REQUIRE_FALSE(a == b);
}

TEST_CASE("operator== returns false for different contents", "[bitset]") {
    DynamicBitset a("101");
    DynamicBitset b("110");

    REQUIRE_FALSE(a == b);
}

TEST_CASE("two empty bitsets are equal", "[bitset]") {
    DynamicBitset a;
    DynamicBitset b;

    REQUIRE(a == b);
}

TEST_CASE("operator== on same contents different construction",
          "[bitset][compare]") {
    DynamicBitset a("10110");
    DynamicBitset b(5);

    b.set(1);
    b.set(2);
    b.set(4);

    REQUIRE(a == b);
    REQUIRE(a.count() == b.count());
}

TEST_CASE("operator== on same bits but one has extra 0s at MSB",
          "[bitset][compare]") {
    DynamicBitset a("101");
    DynamicBitset b("0101");

    REQUIRE_FALSE(a == b);
}
