#include "MinesweeperGrid.h"

#include <random>
#include <vector>
#include <print>
#include <ranges>

MinesweeperGrid::MinesweeperGrid(const unsigned int size)
{
    for (std::size_t i = 0; i < size; i++)
    {
        std::vector<Case> row;
        for (std::size_t j = 0; j < size; j++)
        {
            row.emplace_back();
        }
        cells.push_back(row);
    }
    gen(rd());
    distX(0, cells.size() - 1);
    distY(0, cells.size() - 1);
}

void MinesweeperGrid::print() const noexcept
{
    std::print("Minesweeper grid\n  ");

    for (std::size_t i = 0; i < cells.size(); i++)
    {
        std::print("{0} ", i);
    }
    std::println();

    for (const auto& [i, row] : cells | std::views::enumerate)
    {
        std::print("{0} ", i);
        for (const auto& cell : row)
        {
            if (cell.state == CaseState::Revealed)
            {
                if (cell.isMine)
                {
                    std::print("* ");
                }
                else if (cell.nbNearbyMines > 0)
                {
                    std::print("{0} ", cell.nbNearbyMines);
                }
                else
                {
                    std::print("  ");
                }
            }
            else
            {
                std::print("# ");
            }
        }
        std::println();
    }
}

void MinesweeperGrid::placeMines(const unsigned int nbMines)
{
    unsigned int count = 0;
    while (count < nbMines)
    {
        const unsigned int x = distX(gen);
        const unsigned int y = distY(gen);

        if (!cells.at(x).at(y).isMine)
        {
            cells.at(x).at(y).isMine = true;
            count++;
        }
    }
}

void MinesweeperGrid::calculateNeighboringMines()
{
    for (int i = 0; i < static_cast<int>(cells.size()); i++)
    {
        for (int j = 0; j < static_cast<int>(cells.size()); j++)
        {
            if (cells.at(i).at(j).isMine)
                continue;

            int counter = 0;

            for (int di = std::max(0, i - 1); di < std::min(i + 2, static_cast<int>(cells.size())); di++)
            {
                for (int dj = std::max(0, j - 1); dj < std::min(j + 2, static_cast<int>(cells.size())); dj++)
                {
                    if (di == i && dj == j)
                        continue;

                    if (cells.at(di).at(dj).isMine)
                        counter++;
                }
            }
            cells.at(i).at(j).nbNearbyMines = counter;
        }
    }
}

void MinesweeperGrid::revealCell(const int x, const int y)
{
    std::vector<std::pair<int, int>> stack;
    stack.emplace_back(x, y);

    while (!stack.empty())
    {
        auto [i, j] = stack.back();
        stack.pop_back();

        for (int di = std::max(0, i - 1); di < std::min(i + 2, static_cast<int>(cells.size())); di++)
        {
            for (int dj = std::max(0, j - 1); dj < std::min(j + 2, static_cast<int>(cells.size())); dj++)
            {
                if (di == i && dj == j) continue;

                auto& neighbor = cells.at(di).at(dj);
                if (neighbor.state != CaseState::Revealed)
                {
                    neighbor.state = CaseState::Revealed;
                    if (neighbor.nbNearbyMines == 0 && !neighbor.isMine)
                    {
                        stack.emplace_back(di, dj);
                    }
                }
            }
        }
    }
}

[[nodiscard]] bool MinesweeperGrid::reveal(const int x, const int y)
{
    cells.at(x).at(y).state = CaseState::Revealed;

    if (cells.at(x).at(y).isMine)
    {
        return false;
    }

    if (cells.at(x).at(y).nbNearbyMines == 0)
    {
        revealCell(x, y);
    }
    return true;
}

[[nodiscard]] bool MinesweeperGrid::isWin() const noexcept
{
    for (const auto& row : cells)
    {
        for (const auto& cell : row)
        {
            if (!cell.isMine && cell.state != CaseState::Revealed)
            {
                return false;
            }
        }
    }
    return true;
}
