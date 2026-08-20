#ifndef GRID_H
#define GRID_H

#include <random>
#include <vector>

#include "./Case.hpp"

struct Grid
{
private:
    std::vector<std::vector<Case>> grid;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> distX;
    std::uniform_int_distribution<int> distY;
    [[nodiscard]] bool solveAt(int x, int y, bool randomize, int& nbSolutions, int maxSolutions);
    [[nodiscard]] bool isValidInRow(int x, int y, int value) const;
    [[nodiscard]] bool isValidInColumn(int x, int y, int value) const;
    [[nodiscard]] bool isValidInSquare(int x, int y, int value) const;

public:
    Grid();
    void print() const noexcept;
    void initGrid();
    void generatePuzzle(int nbCaseEmpty);
    [[nodiscard]] bool isValid(int x, int y, int input) const;
    [[nodiscard]] bool win() const noexcept;
    void solve();
    [[nodiscard]] bool setValue(int x, int y, int value);
};

#endif //GRID_H
