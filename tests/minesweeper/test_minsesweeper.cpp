#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "MinesweeperGridTestAccess.hpp"

TEST_CASE("MinesweeperGrid constructor initializes empty grid", "[minesweeper]") {
    MinesweeperGrid grid(5);
    REQUIRE(MinesweeperGridTestAccess::getSize(grid) == 5);
    for (unsigned int i = 0; i < 5; ++i) {
        for (unsigned int j = 0; j < 5; ++j) {
            const Case& cell = MinesweeperGridTestAccess::getCell(grid,i, j);
            REQUIRE(cell.state == CaseState::Hidden);
            REQUIRE_FALSE(cell.isMine);
            REQUIRE(cell.nbNearbyMines == 0);
        }
    }
}

TEST_CASE("placeMines places exactly nbMines mines", "[minesweeper]") {
    MinesweeperGrid grid(10);
    grid.placeMines(10);

    int mineCount = 0;
    for (unsigned int i = 0; i < MinesweeperGridTestAccess::getSize(grid); ++i) {
        for (unsigned int j = 0; j < MinesweeperGridTestAccess::getSize(grid); ++j) {
            if (MinesweeperGridTestAccess::isMine(grid,i, j)) ++mineCount;
        }
    }
    REQUIRE(mineCount == 10);
}

TEST_CASE("calculateNeighboringMines counts adjacent mines", "[minesweeper]") {
    MinesweeperGrid grid(3);
    MinesweeperGridTestAccess::setMine(grid,0, 0, true);
    MinesweeperGridTestAccess::setMine(grid,2, 2, true);
    grid.calculateNeighboringMines();

    // La case (1,1) a deux mines voisines
    REQUIRE(MinesweeperGridTestAccess::getNeighborMines(grid,1, 1) == 2);
    // La case (0,1) a une mine voisine (0,0)
    REQUIRE(MinesweeperGridTestAccess::getNeighborMines(grid,0, 1) == 1);
    // La case (2,2) elle-même est une mine, mais le compteur doit être 0 (ou on ne compte pas la case elle-même)
    REQUIRE(MinesweeperGridTestAccess::getNeighborMines(grid,2, 2) == 0);
}

TEST_CASE("reveal on a mine returns false and reveals the cell", "[minesweeper]") {
    MinesweeperGrid grid(3);
    MinesweeperGridTestAccess::setMine(grid,1, 1, true);
    grid.calculateNeighboringMines();

    bool result = grid.reveal(1, 1);
    REQUIRE_FALSE(result);
    REQUIRE(MinesweeperGridTestAccess::isRevealed(grid,1, 1));
}

TEST_CASE("reveal on a safe cell returns true and reveals it", "[minesweeper]") {
    MinesweeperGrid grid(3);
    MinesweeperGridTestAccess::setMine(grid,0, 0, true);
    grid.calculateNeighboringMines();

    bool result = grid.reveal(1, 1);  // case sûre
    REQUIRE(result);
    REQUIRE(MinesweeperGridTestAccess::isRevealed(grid,1, 1));
}

TEST_CASE("isWin works correctly", "[minesweeper]") {
    MinesweeperGrid grid(3);
    MinesweeperGridTestAccess::setMine(grid,0, 0, true);
    grid.calculateNeighboringMines();

    REQUIRE_FALSE(grid.isWin());

    // Révéler toutes les cases sauf la mine (0,0)
    for (unsigned int i = 0; i < 3; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            if (!(i == 0 && j == 0)) {
                REQUIRE(grid.reveal(i, j));
                REQUIRE(MinesweeperGridTestAccess::isRevealed(grid,i, j));
            }
        }
    }
    REQUIRE(grid.isWin());
}