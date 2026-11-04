#include <catch2/catch_test_macros.hpp>
#include "DynamicBitset.hpp"

TEST_CASE("push_back on empty bitset", "[bitset][push_pop]")
{
    DynamicBitset bs;
    REQUIRE(bs.empty());
    REQUIRE(bs.size() == 0);

    bs.push_back(true);
    REQUIRE(bs.size() == 1);
    REQUIRE(bs.test(0) == true);
    REQUIRE(bs.count() == 1);

    bs.push_back(false);
    REQUIRE(bs.size() == 2);
    REQUIRE(bs.test(1) == false);
    REQUIRE(bs.count() == 1);
}

TEST_CASE("push_back crossing byte boundaries", "[bitset][push_pop]")
{
    DynamicBitset bs;

    // On ajoute 8 bits (ça remplit le premier octet)
    for (int i = 0; i < 8; ++i)
    {
        bs.push_back(true);
    }
    REQUIRE(bs.size() == 8);
    REQUIRE(bs.count() == 8);
    REQUIRE(bs.all());

    // On ajoute un 9ème bit (ça doit allouer un 2ème octet)
    bs.push_back(true);
    REQUIRE(bs.size() == 9);
    REQUIRE(bs.count() == 9);
    REQUIRE(bs.test(8) == true);
    REQUIRE(bs.all());

    // On ajoute un 10ème bit (dans le 2ème octet)
    bs.push_back(false);
    REQUIRE(bs.size() == 10);
    REQUIRE(bs.count() == 9);
    REQUIRE(bs.test(9) == false);
    REQUIRE_FALSE(bs.all());
}

TEST_CASE("push_back preserves previous bits", "[bitset][push_pop]")
{
    DynamicBitset bs("101");
    REQUIRE(bs.size() == 3);

    bs.push_back(true);
    bs.push_back(false);

    REQUIRE(bs.size() == 5);
    REQUIRE(bs.to_string() == "01101");
}

TEST_CASE("pop_back removes the last bit", "[bitset][push_pop]")
{
    DynamicBitset bs("10110"); // 5 bits
    REQUIRE(bs.size() == 5);
    REQUIRE(bs.test(4) == true);

    bs.pop_back();
    REQUIRE(bs.size() == 4);
    REQUIRE(bs.to_string() == "0110");

    bs.pop_back();
    REQUIRE(bs.size() == 3);
    REQUIRE(bs.to_string() == "110");
}

TEST_CASE("pop_back on empty bitset throws", "[bitset][push_pop]")
{
    DynamicBitset bs;
    REQUIRE(bs.empty());
    REQUIRE_THROWS_AS(bs.pop_back(), std::out_of_range);
}

TEST_CASE("pop_back clears the bit to prevent ghosts", "[bitset][push_pop]")
{
    DynamicBitset bs;
    bs.push_back(true); // 1 bit (vaut 1)
    REQUIRE(bs.count() == 1);

    bs.pop_back(); // 0 bits
    REQUIRE(bs.size() == 0);

    bs.push_back(false); // On repousse un bit à 0
    REQUIRE(bs.size() == 1);
    REQUIRE(bs.test(0) == false); // Si on n'a pas reset l'ancien bit, il serait resté à 1 ici !
    REQUIRE(bs.count() == 0);
}

TEST_CASE("pop_back crossing byte boundaries", "[bitset][push_pop]")
{
    DynamicBitset bs(9, true); // 9 bits, tous à 1
    REQUIRE(bs.all());
    REQUIRE(bs.count() == 9);

    bs.pop_back(); // Retire le 9ème bit
    REQUIRE(bs.size() == 8);
    REQUIRE(bs.count() == 8);
    REQUIRE(bs.all()); // Les 8 premiers bits sont toujours à 1

    bs.pop_back(); // Retire le 8ème bit
    REQUIRE(bs.size() == 7);
    REQUIRE(bs.all());
    REQUIRE(bs.count() == 7);
}
