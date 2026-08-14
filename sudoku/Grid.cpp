#include "./Grid.h"
#include "./Case.h"

#include <random>

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

bool Grid::solveAt(int x, int y)
{
    return true;
}

void Grid::print() const noexcept
{

}

void Grid::initGrid()
{
}

bool Grid::isValid(int x, int y, int input) const
{
    return true;
}

bool Grid::win() const noexcept
{
    return true;
}

void Grid::solve()
{

}
