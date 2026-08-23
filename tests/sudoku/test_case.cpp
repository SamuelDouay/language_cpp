#include <catch2/catch_test_macros.hpp>

#include "Cell.hpp"

TEST_CASE("Case has correct default values", "[cell]") {
    Cell cell;

    REQUIRE(cell.value == 0);
    REQUIRE(cell.play == NumberOrigin::FIXED);
}

TEST_CASE("Case fields can be modified", "[cell]")
{
    Cell cell;
    cell.value = 7;
    cell.play = NumberOrigin::PLAYER;

    REQUIRE(cell.value == 7u);
    REQUIRE(cell.play == NumberOrigin::PLAYER);
}

TEST_CASE("Case can be aggregate-initialized", "[cell]")
{
    Cell cell{5, NumberOrigin::PLAYER};

    REQUIRE(cell.value == 5u);
    REQUIRE(cell.play == NumberOrigin::PLAYER);
}

TEST_CASE("NumberOrigin enum values are distinct", "[cell]")
{
    REQUIRE(static_cast<int>(NumberOrigin::FIXED) != static_cast<int>(NumberOrigin::PLAYER));
}