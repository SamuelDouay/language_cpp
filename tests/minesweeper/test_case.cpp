#include <catch2/catch_test_macros.hpp>
#include "Case.hpp"

TEST_CASE("Case has correct default values", "[case]")
{
    Cell cell;
    REQUIRE(cell.state == CaseState::Hidden);
    REQUIRE_FALSE(cell.isMine);
    REQUIRE(cell.nbNearbyMines == 0);
}

TEST_CASE("Case attributes can be modified", "[case]")
{
    Cell cell;
    cell.state = CaseState::Revealed;
    cell.isMine = true;
    cell.nbNearbyMines = 3;

    REQUIRE(cell.state == CaseState::Revealed);
    REQUIRE(cell.isMine);
    REQUIRE(cell.nbNearbyMines == 3);
}

TEST_CASE("CaseState enum values are distinct", "[case]")
{
    REQUIRE(static_cast<int>(CaseState::Hidden) != static_cast<int>(CaseState::Revealed));
    REQUIRE(static_cast<int>(CaseState::Hidden) != static_cast<int>(CaseState::Flagged));
    REQUIRE(static_cast<int>(CaseState::Revealed) != static_cast<int>(CaseState::Flagged));
}