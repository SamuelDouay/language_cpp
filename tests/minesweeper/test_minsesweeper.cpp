#include <catch2/catch_test_macros.hpp>

#include "MinesweeperGrid.hpp"

TEST_CASE("Minesweeper init", "[minesweeper]") {
    MinesweeperGrid grid(10);

    REQUIRE(grid.isWin() == false);

}