#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("resize can grow a bitset", "[bitset][resize]")
{
    DynamicBitset bs("101"); // 3 bits
    bs.resize(5, true); // Agrandit à 5 bits, les nouveaux sont à 1

    REQUIRE(bs.size() == 5);
    REQUIRE(bs.to_string() == "11101"); // 2 '1' ajoutés à gauche
    REQUIRE(bs.count() == 4);
}

TEST_CASE("resize can shrink a bitset", "[bitset][resize]")
{
    DynamicBitset bs("10110"); // 5 bits
    bs.resize(3); // Rétrécit à 3 bits (le paramètre value est ignoré)

    REQUIRE(bs.size() == 3);
    REQUIRE(bs.to_string() == "110"); // On a gardé les 3 bits de droite
}

TEST_CASE("resize to same size does nothing", "[bitset][resize]")
{
    DynamicBitset bs("101");
    bs.resize(3);
    REQUIRE(bs.size() == 3);
    REQUIRE(bs.to_string() == "101");
}

TEST_CASE("resize handles multiple byte boundaries", "[bitset][resize]")
{
    DynamicBitset bs;
    bs.resize(20, false); // 3 octets alloués
    REQUIRE(bs.size() == 20);
    REQUIRE(bs.none());

    bs.resize(5); // Rétrécit à 5 bits
    REQUIRE(bs.size() == 5);
    REQUIRE(bs.none());
}

TEST_CASE("clear empties the bitset", "[bitset][clear]")
{
    DynamicBitset bs("10110");
    REQUIRE(bs.size() == 5);

    bs.clear();
    REQUIRE(bs.size() == 0);
    REQUIRE(bs.empty());
    REQUIRE(bs.none());

    // On peut réutiliser le bitset après un clear
    bs.push_back(true);
    REQUIRE(bs.size() == 1);
    REQUIRE(bs.test(0) == true);
}
