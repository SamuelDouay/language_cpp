#include "Grid.hpp"
#include "catch2/catch_test_macros.hpp"


TEST_CASE("Sudoku") {
    Grid grid;

    REQUIRE(grid.isWin() == false);
}