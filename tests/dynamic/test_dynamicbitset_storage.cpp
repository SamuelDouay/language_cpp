#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("bits are packed efficiently", "[bitset]") {
    // 9 bits doivent tenir dans 2 octets
    DynamicBitset bs(9);

    bs.set(0);
    bs.set(8);

    REQUIRE(bs.count() == 2);
    REQUIRE(bs.test(0));
    REQUIRE(bs.test(8));
    REQUIRE_FALSE(bs.test(1));
}

TEST_CASE("bitset with size 1", "[bitset]") {
    DynamicBitset bs(1);

    REQUIRE(bs.size() == 1);
    REQUIRE(bs.none());

    bs.set(0);

    REQUIRE(bs.all());
    REQUIRE(bs.count() == 1);
}

TEST_CASE("bitset crossing multiple bytes", "[bitset]") {
    DynamicBitset bs(20);

    for (std::size_t i = 0; i < 20; i += 3)
        bs.set(i);

    REQUIRE(bs.count() == 7);

    for (std::size_t i = 0; i < 20; i += 3)
        REQUIRE(bs.test(i));

    for (std::size_t i = 1; i < 20; i += 3)
        REQUIRE_FALSE(bs.test(i));
}

TEST_CASE("bitset stores multiple bits per byte", "[bitset][storage]") {
    // 8 bits doivent tenir dans 1 octet, pas 8
    DynamicBitset bs(8);

    bs.set(0);
    bs.set(3);
    bs.set(7);

    REQUIRE(bs.count() == 3);
    REQUIRE(bs.test(0));
    REQUIRE(bs.test(3));
    REQUIRE(bs.test(7));
    REQUIRE_FALSE(bs.test(1));
}

TEST_CASE("bitset of size 16 fits in 2 bytes worth of bits",
          "[bitset][storage]") {
    DynamicBitset bs(16);

    for (std::size_t i = 0; i < 16; i += 2)
        bs.set(i);

    REQUIRE(bs.count() == 8);

    for (std::size_t i = 0; i < 16; i += 2)
        REQUIRE(bs.test(i));
}
