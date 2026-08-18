#include "./Grid.hpp"
#include "./Case.hpp"

#include <print>
#include <random>
#include <ranges>

Grid::Grid()
{
    for (int i = 0; i < 9; i++)
    {
        std::vector<Case> row;

        for (int j = 0; j < 9; j++)
        {
            row.emplace_back();
        }
        grid.push_back(row);
    }
}

bool Grid::solveAt(const int x,const int y)
{
    if (x ==  9) {
        return true;
    }
    if (y == 9) {
        return solveAt(x + 1, 0);
    }
    if (grid.at(x).at(y).value != 0) {
        return solveAt(x, y + 1);
    }

    for (int number = 1; number < 10; number++) {
        if (isValid(x, y, number)) {
            grid.at(x).at(y).value = number;
            if (solveAt(x, y + 1)) {
                return true;
            }
            grid.at(x).at(y).value = 0;
        }
    }
    return false;
}

bool Grid::isValidInRow( const int x,const int y, const int value) const {
    for (int column = 0; column < 9; column++) {
        if (column != y && grid.at(x).at(column).value == value) {
            return false;
        }
    }
    return true;
}

bool Grid::isValidInColumn(const int x,const int y, const int value) const {
    for (int row = 0; row < 9; row++) {
        if (row != x && grid.at(row).at(y).value == value) {
            return false;
        }
    }
    return true;
}

bool Grid::isValidInSquare(const int x,const int y, const int value) const {
    const int startRow = (x / 3) * 3;
    const int startCol = (y / 3) * 3;

    for (int row = startRow; row < startRow + 3; row++) {
        for (int column = startCol; column <  startCol + 3; column++) {
            if ( x == row && y == column) {
                continue;
            }
            if (grid.at(row).at(column).value == value) {
                return false;
            }
        }
    }
    return true;
}

void Grid::print() const noexcept
{
    std::println("Sudoku grid");

    std::print("  | ");
    for (int i = 0; i < 9; i++) {
        std::print("{0} ", i);
        if (i % 3 == 2) {
            std::print("| ");
        }
    }
    std::println();
    std::print("- | ");
    for (int i = 0; i < 9; i++) {
        std::print("- ");
        if (i % 3 == 2) {
            std::print("| ");
        }
    }
    std::println();
    for ( const auto &[i, row] : grid | std::views::enumerate) {
        std::print("{0} | ", i);
        for (const auto &[j, cell] : row | std::views::enumerate) {
            std::print("{0} ", cell.value);
            if (j % 3 == 2) {
                std::print("| ");
            }
        }
        std::println();
        if (i % 3 == 2) {
            std::print("- | ");
            for (int j = 0; j < 9; j++) {
                std::print("- ");
                if (j % 3 == 2) {
                    std::print("| ");
                }
            }
            std::println();
        }
    }
}

void Grid::initGrid()
{
}

bool Grid::isValid(const int x,const int y,const int input) const
{
    return isValidInColumn(x, y, input) && isValidInRow(x, y, input) && isValidInSquare(x, y, input);
}

bool Grid::win() const noexcept
{
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            if (cell.value == 0) {
                return false;
            }
        }
    }
    return true;
}


void Grid::solve()
{
    if (solveAt(0, 0)) {
        std::print("Grid solved");
    }
}
