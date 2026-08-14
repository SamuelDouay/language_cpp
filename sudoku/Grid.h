#ifndef GRID_H
#define GRID_H

#include <vector>

#include "./Case.h"

struct Grid
{
private:
    std::vector<std::vector<Case>> grid;
    [[nodiscard]] bool solveAt(int x, int y);

public:
    Grid();
    void print() const noexcept;
    void initGrid();
    [[nodiscard]] bool isValid(int x, int y, int input) const;
    [[nodiscard]] bool win() const noexcept;
    void solve();
};

#endif //GRID_H
