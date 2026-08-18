#include <iostream>
#include <print>
#include <limits>

#include "MinesweeperGrid.hpp"

static std::optional<int> getCoordinateCell(const std::string& name, const unsigned int size)
{
    int number;
    std::print("Enter {1} coordinate between 0 and {0}: ", size - 1, name);
    while (!(std::cin >> number) || number < 0 || number >= static_cast<int>(size))
    {
        if (std::cin.eof())
        {
            std::println("\nInput stream closed. Exiting.");
            return {};
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::print("Invalid input. Enter {1} between 0 and {0}: ", size - 1, name);
    }
    return number;
}

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
        grid.print();

        std::optional<int> x = getCoordinateCell("x", size);
        std::optional<int> y = getCoordinateCell("y", size);

        if (!x || !y)
        {
            std::println("Aborting game.");
            return 0;
        }

        if (!grid.reveal(x.value(), y.value()))
        {
            gameOver = true; // hit a mine
        }
        else if (grid.isWin())
        {
            gameOver = true;
            won = true;
        }
    }

    grid.print();

    if (won)
        std::println("Congratulations, you won!");
    else
        std::println("Boom, you lost...");

    return 0;
}
