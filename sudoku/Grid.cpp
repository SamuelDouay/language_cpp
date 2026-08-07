#include "./sudoku/Case.h"
#include "./sudoku/Grid.h"

#include <cstdio>
#include <random>

Grid::Grid(int taille)
{
    for (int i = 0; i < taille; i++)
    {
        std::vector<std::unique_ptr<Case>> ligne;

        for (int j = 0; j < taille; j++)
        {
            ligne.push_back(std::make_unique<Case>());
        }
        this->grid.push_back(std::move(ligne));
    }
}

void Grid::print()
{
    std::printf("  ");
    for (int i = 0; i < this->grid.size(); i++)
    {
        std::printf("%i ", i);
    }
    std::printf("\n");

    for (int i = 0; i < this->grid.size(); i++)
    {
        std::printf("%i ", i);
        for (int j = 0; j < this->grid[i].size(); j++)
        {
            std::printf("%i ", this->grid[i][j]->realValue);
        }
        std::printf("\n");
    }
}

void Grid::initGrid()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distX(0, this->grid.size() - 1);
    std::uniform_int_distribution<int> distY(0, this->grid.size() - 1);
    std::uniform_int_distribution<int> number(1, 9);

    int x = distX(gen), y = distY(gen), value = number(gen);

    std::printf("x : %i, y : %i, number : %i  \n", x, y, value);

    if (this->grid[x][y]->realValue == 0)
    {
        this->grid[x][y]->realValue = value;
    }

    for (auto & i : this->grid)
    {
        for (const auto & j : i)
        {
            if (j->realValue == 0)
            {
                const int a = number(gen);
                j->realValue = a;
            }
        }
    }
}
