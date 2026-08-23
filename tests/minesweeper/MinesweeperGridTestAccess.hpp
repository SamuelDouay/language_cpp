#ifndef MINESWEEPER_GRID_TEST_ACCESS_H
#define MINESWEEPER_GRID_TEST_ACCESS_H

#include "MinesweeperGrid.hpp"

struct MinesweeperGridTestAccess
{
    [[nodiscard]] static unsigned int getSize(const MinesweeperGrid& g)
    {
        return static_cast<unsigned int>(g.cells.size());
    }

    [[nodiscard]] static const Case& getCell(const MinesweeperGrid& g, unsigned int x, unsigned int y)
    {
        return g.cells.at(x).at(y);
    }

    [[nodiscard]] static bool isMine(const MinesweeperGrid& g, unsigned int x, unsigned int y)
    {
        return g.cells.at(x).at(y).isMine;
    }

    [[nodiscard]] static unsigned int getNeighborMines(const MinesweeperGrid& g, unsigned int x, unsigned int y)
    {
        return g.cells.at(x).at(y).nbNearbyMines;
    }

    [[nodiscard]] static bool isRevealed(const MinesweeperGrid& g, unsigned int x, unsigned int y)
    {
        return g.cells.at(x).at(y).state == CaseState::Revealed;
    }

    static void setMine(MinesweeperGrid& g, unsigned int x, unsigned int y, bool value)
    {
        g.cells.at(x).at(y).isMine = value;
    }
};

#endif // MINESWEEPER_GRID_TEST_ACCESS_H