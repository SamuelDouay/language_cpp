#include "Grid.hpp"
#include "Cell.hpp"

#include <print>
#include <random>
#include <ranges>
#include <array>
#include <algorithm>

Grid::Grid()
{
    for (unsigned int i = 0; i < 9; i++)
    {
        std::vector<Cell> row;

        for (unsigned int j = 0; j < 9; j++)
        {
            row.emplace_back();
        }
        grid.push_back(row);
    }

    gen = std::mt19937(rd());
    distX = std::uniform_int_distribution<unsigned int>(0, 8);
    distY = std::uniform_int_distribution<unsigned int>(0, 8);
}

bool Grid::solveAt(const unsigned int x, const unsigned int y, const bool randomize, unsigned& nbSolutions,
                   const unsigned int maxSolutions)
{
    if (x == 9)
    {
        nbSolutions++;
        if (nbSolutions == maxSolutions)
        {
            return true;
        }
        return nbSolutions >= maxSolutions;
    }
    if (y == 9)
    {
        return solveAt(x + 1, 0, randomize, nbSolutions, maxSolutions);
    }
    if (grid.at(x).at(y).value != 0)
    {
        return solveAt(x, y + 1, randomize, nbSolutions, maxSolutions);
    }

    std::array<unsigned int, 9> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    if (randomize)
    {
        std::shuffle(std::begin(numbers), std::end(numbers), gen);
    }

    for (const unsigned int num : numbers)
    {
        if (isValid(x, y, num))
        {
            grid.at(x).at(y).value = num;
            if (solveAt(x, y + 1, randomize, nbSolutions, maxSolutions))
            {
                return true;
            }
            grid.at(x).at(y).value = 0;
        }
    }
    return false;
}

bool Grid::isValidInRow(const unsigned int x, const unsigned int y, const unsigned int value) const
{
    for (unsigned int column = 0; column < 9; column++)
    {
        if (column != y && grid.at(x).at(column).value == value)
        {
            return false;
        }
    }
    return true;
}

bool Grid::isValidInColumn(const unsigned int x, const unsigned int y, const unsigned int value) const
{
    for (unsigned int row = 0; row < 9; row++)
    {
        if (row != x && grid.at(row).at(y).value == value)
        {
            return false;
        }
    }
    return true;
}

bool Grid::isValidInSquare(const unsigned int x, const unsigned int y, const unsigned int value) const
{
    const unsigned int startRow = (x / 3) * 3;
    const unsigned int startCol = (y / 3) * 3;

    for (unsigned int row = startRow; row < startRow + 3; row++)
    {
        for (unsigned int column = startCol; column < startCol + 3; column++)
        {
            if (x == row && y == column)
            {
                continue;
            }
            if (grid.at(row).at(column).value == value)
            {
                return false;
            }
        }
    }
    return true;
}

void Grid::printSeparator() const noexcept
{
    std::print("\033[33m- | \033[m");
    for (unsigned int i = 0; i < 9; i++)
    {
        std::print("\033[33m- \033[m");
        if (i % 3 == 2)
        {
            std::print("\033[33m| \033[m");
        }
    }
    std::println();
}

void Grid::print() const noexcept
{
    std::println("Sudoku grid");

    std::print("\033[33m  | \033[m");
    for (unsigned int i = 0; i < 9; i++)
    {
        std::print("\033[33m{0} \033[m", i);
        if (i % 3 == 2)
        {
            std::print("\033[33m| \033[m", i);
        }
    }
    std::println();
    printSeparator();
    for (const auto& [i, row] : grid | std::views::enumerate)
    {
        std::print("\033[33m{0} | \033[m", i);
        for (const auto& [j, cell] : row | std::views::enumerate)
        {
            if (cell.value == 0)
            {
                std::print("  ", i);
            }
            else
            {
                if (cell.play == NumberOrigin::PLAYER)
                {
                    std::print("{0} ", cell.value);
                }
                else
                {
                    std::print("\033[36m{0} \033[m", cell.value);
                }
            }
            if (j % 3 == 2)
            {
                std::print("\033[33m| \033[m");
            }
        }
        std::println();
        if (i % 3 == 2)
        {
            printSeparator();
        }
    }
    std::println();
}

void Grid::initGrid()
{
    for (auto& row : grid)
    {
        for (auto& cell : row)
        {
            cell.value = 0;
            cell.play = NumberOrigin::FIXED;
        }
    }
    unsigned int nbSolution = 0;
    if (solveAt(0, 0, true, nbSolution, 1))
    {
        solved = grid;
    }
}

void Grid::generatePuzzle(const unsigned int nbCaseEmpty)
{
    initGrid();

    unsigned int empty = 0;
    while (empty < nbCaseEmpty)
    {
        unsigned int x = distX(gen);
        unsigned int y = distY(gen);

        if (grid.at(x).at(y).value == 0)
        {
            continue;
        }

        const auto copyGrid = grid;
        grid.at(x).at(y).value = 0;
        unsigned int nbSolution = 0;
        if (!solveAt(0, 0, false, nbSolution, 2))
        {
            grid = copyGrid;
            grid.at(x).at(y).value = 0;
            grid.at(x).at(y).play = NumberOrigin::PLAYER;
            empty++;
        }
        else
        {
            grid = copyGrid;
        }
    }
}

bool Grid::isValid(const unsigned int x, const unsigned int y, const unsigned int input) const
{
    return isValidInColumn(x, y, input) && isValidInRow(x, y, input) && isValidInSquare(x, y, input);
}

bool Grid::win() const noexcept
{
    for (const auto& row : grid)
    {
        for (const auto& cell : row)
        {
            if (cell.value == 0)
            {
                return false;
            }
        }
    }
    return true;
}

bool Grid::solve()
{
    unsigned int nbsolution = 0;
    return solveAt(0, 0, false, nbsolution, 1);
}

bool Grid::isEditable(const unsigned int x, const unsigned int y) const
{
    return grid.at(x).at(y).play == NumberOrigin::PLAYER;
}

bool Grid::isCorrect(const unsigned int x, const unsigned int y, const unsigned int value) const
{
    return solved.at(x).at(y).value == value;
}

void Grid::setValue(const unsigned int x, const unsigned int y, const unsigned int value)
{
    grid.at(x).at(y).value = value;
}
