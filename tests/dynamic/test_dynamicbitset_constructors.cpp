#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("default constructor creates empty bitset", "[bitset]") {
    DynamicBitset bs;
    REQUIRE(bs.size() == 0);
    REQUIRE(bs.empty());
    REQUIRE(bs.count() == 0);
    REQUIRE(bs.none());
    REQUIRE_FALSE(bs.any());
}

TEST_CASE("constructor with size initializes all bits to 0", "[bitset]") {
    DynamicBitset bs(10);
    REQUIRE(bs.size() == 10);
    REQUIRE_FALSE(bs.empty());
    REQUIRE(bs.count() == 0);
    REQUIRE(bs.none());

    for (std::size_t i = 0; i < 10; ++i)
        REQUIRE(bs.test(i) == false);
}

TEST_CASE("constructor with size and value sets all bits", "[bitset]") {
    DynamicBitset bs(8, true);
    REQUIRE(bs.size() == 8);
    REQUIRE(bs.count() == 8);
    REQUIRE(bs.all());

    for (std::size_t i = 0; i < 8; ++i)
        REQUIRE(bs.test(i) == true);
}

TEST_CASE("constructor with size and false sets all bits to 0", "[bitset]") {
    DynamicBitset bs(8, false);
    REQUIRE(bs.count() == 0);
    REQUIRE(bs.none());
}

TEST_CASE("constructor from string", "[bitset]") {
    DynamicBitset bs("10110");
    REQUIRE(bs.size() == 5);
    REQUIRE(bs.test(0) == false); // LSB
    REQUIRE(bs.test(1) == true);
    REQUIRE(bs.test(2) == true);
    REQUIRE(bs.test(3) == false);
    REQUIRE(bs.test(4) == true); // MSB
}

TEST_CASE("constructor from empty string", "[bitset]") {
    DynamicBitset bs("");
    REQUIRE(bs.size() == 0);
    REQUIRE(bs.empty());
}

TEST_CASE("constructor from string throws on invalid char", "[bitset]") {
    REQUIRE_THROWS_AS(DynamicBitset("10x10"), std::invalid_argument);
}

TEST_CASE("constructor from string sets exact bits",
          "[bitset][constructor][string]") {
    DynamicBitset bs("10010");
    REQUIRE(bs.size() == 5);
    REQUIRE(bs.count() == 2);
    REQUIRE(bs.test(1));
    REQUIRE(bs.test(4));
    REQUIRE_FALSE(bs.test(0));
    REQUIRE_FALSE(bs.test(2));
    REQUIRE_FALSE(bs.test(3));
}

TEST_CASE("constructor from single char strings",
          "[bitset][constructor][string]") {
    DynamicBitset zero("0");
    REQUIRE(zero.size() == 1);
    REQUIRE(zero.count() == 0);
    REQUIRE(zero.test(0) == false);

    DynamicBitset one("1");
    REQUIRE(one.size() == 1);
    REQUIRE(one.count() == 1);
    REQUIRE(one.test(0) == true);
}
