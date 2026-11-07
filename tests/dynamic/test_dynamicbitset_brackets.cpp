#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("operator[] allows reading bits", "[bitset][brackets]")
{
    const DynamicBitset bs("10110");
    REQUIRE(bs[0] == false);
    REQUIRE(bs[1] == true);
    REQUIRE(bs[2] == true);
    REQUIRE(bs[3] == false);
    REQUIRE(bs[4] == true);
}

TEST_CASE("operator[] allows writing bits", "[bitset][brackets]")
{
    DynamicBitset bs(8);
    REQUIRE(bs.none());

    bs[3] = true;
    REQUIRE(bs.test(3) == true);
    REQUIRE(bs.count() == 1);

    bs[3] = false;
    REQUIRE(bs.test(3) == false);
    REQUIRE(bs.count() == 0);
}

TEST_CASE("operator[] allows assigning one bit to another", "[bitset][brackets]")
{
    DynamicBitset bs("1011"); // indices 0, 1, 2, 3

    // On assigne le bit 3 au bit 0
    bs[0] = bs[3];
    REQUIRE(bs.to_string() == "1011"); // Pas de changement (tous les deux à 1)

    // On assigne le bit 1 au bit 2
    bs[2] = bs[1]; // bit 1 est à 1, on le met dans le bit 2
    REQUIRE(bs.test(2) == true);
}

TEST_CASE("operator[] allows flipping through proxy", "[bitset][brackets]")
{
    DynamicBitset bs(8);
    bs[2] = true;
    REQUIRE(bs[2] == true);

    // Si tu implémentes flip() sur le Proxy
    bs[2].flip();
    REQUIRE(bs[2] == false);
}
