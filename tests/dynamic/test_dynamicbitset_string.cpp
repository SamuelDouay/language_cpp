#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("to_string on simple values", "[bitset]") {
    DynamicBitset bs(4);
    REQUIRE(bs.to_string() == "0000");

    bs.set(0);
    REQUIRE(bs.to_string() == "0001");

    bs.set(3);
    REQUIRE(bs.to_string() == "1001");
}

TEST_CASE("to_string round-trips with constructor", "[bitset]") {
    DynamicBitset bs("11010010");
    REQUIRE(bs.to_string() == "11010010");
}

TEST_CASE("to_string on empty bitset", "[bitset]") {
    DynamicBitset bs;
    REQUIRE(bs.to_string() == "");
}

TEST_CASE("to_string across byte boundaries", "[bitset]") {
    DynamicBitset bs(12);

    bs.set(0);
    bs.set(7);
    bs.set(8);
    bs.set(11);

    REQUIRE(bs.to_string() == "100110000001");
}

TEST_CASE("to_string with one bit set at LSB", "[bitset][to_string]") {
    DynamicBitset bs(4);
    bs.set(0);

    REQUIRE(bs.to_string() == "0001");
}

TEST_CASE("to_string with one bit set at MSB", "[bitset][to_string]") {
    DynamicBitset bs(4);
    bs.set(3);

    REQUIRE(bs.to_string() == "1000");
}

TEST_CASE("to_string with alternating bits", "[bitset][to_string]") {
    DynamicBitset bs(8);

    bs.set(0);
    bs.set(2);
    bs.set(4);
    bs.set(6);

    REQUIRE(bs.to_string() == "01010101");
}

TEST_CASE("to_string with all bits set", "[bitset][to_string]") {
    DynamicBitset bs(5);
    bs.set();

    REQUIRE(bs.to_string() == "11111");
}

TEST_CASE("to_string with no bits set", "[bitset][to_string]") {
    DynamicBitset bs(5);

    REQUIRE(bs.to_string() == "00000");
}

TEST_CASE("to_string preserves string round-trip", "[bitset][to_string]") {
    const char *inputs[] = {
        "1", "0", "10", "101", "1111",
        "10110", "10000001", "11111111"
    };

    for (const char *s: inputs) {
        DynamicBitset bs(s);
        REQUIRE(bs.to_string() == s);
    }
}

TEST_CASE("to_string across byte boundary 8/9", "[bitset][to_string]") {
    DynamicBitset bs(9);

    bs.set(0);
    bs.set(8);

    REQUIRE(bs.to_string() == "100000001");
}

TEST_CASE("to_string across byte boundary 15/16/17",
          "[bitset][to_string]") {
    DynamicBitset bs(17);

    bs.set(0);
    bs.set(16);

    REQUIRE(bs.to_string() == "10000000000000001");
}
