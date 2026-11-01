#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("count returns number of set bits", "[bitset]") {
    DynamicBitset bs("10110");
    REQUIRE(bs.count() == 3);
}

TEST_CASE("all returns true when all bits set", "[bitset]") {
    DynamicBitset bs(8, true);
    REQUIRE(bs.all());

    bs.reset(2);
    REQUIRE_FALSE(bs.all());
}

TEST_CASE("any returns true if at least one bit set", "[bitset]") {
    DynamicBitset bs(8);
    REQUIRE_FALSE(bs.any());

    bs.set(5);
    REQUIRE(bs.any());
}

TEST_CASE("none returns true when no bit set", "[bitset]") {
    DynamicBitset bs(8);
    REQUIRE(bs.none());

    bs.set(0);
    REQUIRE_FALSE(bs.none());
}

TEST_CASE("all/any/none on empty bitset", "[bitset]") {
    DynamicBitset bs;
    REQUIRE(bs.all());
    REQUIRE_FALSE(bs.any());
    REQUIRE(bs.none());
}

TEST_CASE("all() is false when one bit is 0", "[bitset][queries]") {
    DynamicBitset bs(8, true);
    bs.reset(0);
    REQUIRE_FALSE(bs.all());
}

TEST_CASE("any() is true with single bit", "[bitset][queries]") {
    DynamicBitset bs(8);
    bs.set(7);
    REQUIRE(bs.any());
    REQUIRE_FALSE(bs.none());
}

TEST_CASE("none() is true after reset()", "[bitset][queries]") {
    DynamicBitset bs(8, true);
    bs.reset();
    REQUIRE(bs.none());
    REQUIRE_FALSE(bs.any());
}

TEST_CASE("all/any/none consistency after multiple ops",
          "[bitset][queries]") {
    DynamicBitset bs(8);
    REQUIRE(bs.none());
    REQUIRE_FALSE(bs.any());
    REQUIRE_FALSE(bs.all());

    bs.set();
    REQUIRE(bs.all());
    REQUIRE(bs.any());
    REQUIRE_FALSE(bs.none());

    bs.reset(3);
    REQUIRE_FALSE(bs.all());
    REQUIRE(bs.any());
    REQUIRE_FALSE(bs.none());
}
