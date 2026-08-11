#include <iostream>
#include <print>
#include <limits>

#include "MinesweeperGrid.h"

int main()
{
    constexpr unsigned int size = 10;
    constexpr unsigned int nbMines = 15;

    MinesweeperGrid grid(size);
    grid.placeMines(nbMines);
    grid.calculateNeighboringMines();

    bool gameOver = false;
    bool won = false;

    while (!gameOver)
    {
        int x, y;

        grid.print();

        std::print("Enter x coordinate between 0 and {0}: ", size - 1);
        while (!(std::cin >> x) || x < 0 || x >= static_cast<int>(size))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::print("Invalid input. Enter x between 0 and {0}: ", size - 1);
        }

        std::print("Enter y coordinate between 0 and {0}: ", size - 1);
        while (!(std::cin >> y) || y < 0 || y >= static_cast<int>(size))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::print("Invalid input. Enter y between 0 and {0}: ", size - 1);
        }

        if (!grid.reveal(x, y))
        {
            gameOver = true; // hit a mine
        }
        else if (grid.isWin())
        {
            gameOver = true;
            won = true;
        }

        std::cin.clear();
    }

    grid.print();

    if (won)
        std::println("Congratulations, you won!");
    else
        std::println("Boom, you lost...");

    return 0;
}