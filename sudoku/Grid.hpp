#ifndef GRID_H
#define GRID_H

#include <vector>

#include "./Case.hpp"

struct Grid
{
private:
    std::vector<std::vector<Case>> grid;
    [[nodiscard]] bool solveAt(int x, int y);
    [[nodiscard]] bool isValidInRow(int x, int y, int value) const;
    [[nodiscard]] bool isValidInColumn(int x, int y, int value)const;
    [[nodiscard]] bool isValidInSquare(int x, int y, int value)const;

public:
    Grid();
    void print() const noexcept;
    void initGrid();
    [[nodiscard]] bool isValid(int x, int y, int input) const;
    [[nodiscard]] bool win() const noexcept;
    void solve();
};

#endif //GRID_H
