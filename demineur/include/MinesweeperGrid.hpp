#ifndef MINESWEEPER_GRID_H
#define MINESWEEPER_GRID_H

#include <random>
#include <vector>
#include "Case.hpp"

struct MinesweeperGrid
{
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<unsigned int> distX;
    std::uniform_int_distribution<unsigned int> distY;
    std::vector<std::vector<Case>> cells;
    void revealCell(int x, int y);

public:
    explicit MinesweeperGrid(unsigned int size);
    void print() const noexcept;
    void placeMines(unsigned int nbMines);
    void calculateNeighboringMines();
    [[nodiscard]] bool reveal(int x, int y);
    [[nodiscard]] bool isWin() const noexcept;
};

#endif // MINESWEEPER_GRID_H
