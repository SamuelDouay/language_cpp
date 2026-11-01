#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("set(pos) sets a single bit", "[bitset]") {
    DynamicBitset bs(8);
    bs.set(3);
    REQUIRE(bs.test(3) == true);
    REQUIRE(bs.count() == 1);
}

TEST_CASE("set(pos, value) sets specific value", "[bitset]") {
    DynamicBitset bs(8);
    bs.set(3, true);
    REQUIRE(bs.test(3) == true);

    bs.set(3, false);
    REQUIRE(bs.test(3) == false);
}

TEST_CASE("reset(pos) clears a single bit", "[bitset]") {
    DynamicBitset bs(8, true);
    bs.reset(3);
    REQUIRE(bs.test(3) == false);
    REQUIRE(bs.count() == 7);
}

TEST_CASE("flip(pos) toggles a single bit", "[bitset]") {
    DynamicBitset bs(8);
    bs.flip(3);
    REQUIRE(bs.test(3) == true);

    bs.flip(3);
    REQUIRE(bs.test(3) == false);
}

TEST_CASE("set/reset/flip throw out_of_range on invalid index", "[bitset]") {
    DynamicBitset bs(4);
    REQUIRE_THROWS_AS(bs.set(4), std::out_of_range);
    REQUIRE_THROWS_AS(bs.reset(4), std::out_of_range);
    REQUIRE_THROWS_AS(bs.flip(4), std::out_of_range);
}

TEST_CASE("set() sets all bits", "[bitset]") {
    DynamicBitset bs(10);
    bs.set();
    REQUIRE(bs.all());
    REQUIRE(bs.count() == 10);
}

TEST_CASE("reset() clears all bits", "[bitset]") {
    DynamicBitset bs(10, true);
    bs.reset();
    REQUIRE(bs.none());
    REQUIRE(bs.count() == 0);
}

TEST_CASE("flip() toggles all bits", "[bitset]") {
    DynamicBitset bs(8);
    bs.set(0);
    bs.flip();
    REQUIRE(bs.count() == 7);
    REQUIRE_FALSE(bs.test(0));
    REQUIRE(bs.test(1));
}

TEST_CASE("set twice on same bit does not increase count",
          "[bitset][count]") {
    DynamicBitset bs(8);
    bs.set(3);
    REQUIRE(bs.count() == 1);

    bs.set(3);
    REQUIRE(bs.count() == 1);
}

TEST_CASE("reset twice on same bit does not go negative or wrong",
          "[bitset][count]") {
    DynamicBitset bs(8, true);
    bs.reset(3);
    REQUIRE(bs.count() == 7);

    bs.reset(3);
    REQUIRE(bs.count() == 7);
}

TEST_CASE("set then reset on same bit returns to initial count",
          "[bitset][count]") {
    DynamicBitset bs(8);
    bs.set(3);
    REQUIRE(bs.count() == 1);

    bs.reset(3);
    REQUIRE(bs.count() == 0);
}

TEST_CASE("flip twice on same bit returns to initial state",
          "[bitset][count]") {
    DynamicBitset bs(8);
    bs.flip(3);
    REQUIRE(bs.count() == 1);

    bs.flip(3);
    REQUIRE(bs.count() == 0);
}

TEST_CASE("set(pos, true) on already set bit does not change count",
          "[bitset][count]") {
    DynamicBitset bs(8);
    bs.set(3, true);
    REQUIRE(bs.count() == 1);

    bs.set(3, true);
    REQUIRE(bs.count() == 1);
}

TEST_CASE("set(pos, false) on already cleared bit does not change count",
          "[bitset][count]") {
    DynamicBitset bs(8);
    bs.set(3, false);
    REQUIRE(bs.count() == 0);

    bs.set(3, false);
    REQUIRE(bs.count() == 0);
}

TEST_CASE("set one bit then check count", "[bitset][count]") {
    DynamicBitset bs(10);
    REQUIRE(bs.count() == 0);

    bs.set(5);
    REQUIRE(bs.count() == 1);

    bs.set(2);
    REQUIRE(bs.count() == 2);

    bs.reset(5);
    REQUIRE(bs.count() == 1);
}

TEST_CASE("set() on all bits then reset one bit",
          "[bitset][count]") {
    DynamicBitset bs(8);
    bs.set();
    REQUIRE(bs.count() == 8);

    bs.reset(4);
    REQUIRE(bs.count() == 7);
    REQUIRE_FALSE(bs.test(4));
}

TEST_CASE("flip() after set() reverses everything",
          "[bitset][count]") {
    DynamicBitset bs(8, true);
    REQUIRE(bs.count() == 8);

    bs.flip();
    REQUIRE(bs.count() == 0);
    REQUIRE(bs.none());
}

TEST_CASE("flip() twice after set() restores state",
          "[bitset][count]") {
    DynamicBitset bs(8, true);
    bs.flip();
    bs.flip();

    REQUIRE(bs.count() == 8);
    REQUIRE(bs.all());
}
