#ifdef _WIN32
#include <windows.h>
#endif
#include <optional>
#include <print>
#include <iostream>
#include <string>
#include <limits>

#include "Grid.hpp"

#ifdef _WIN32
void enableVirtualTerminalProcessing()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#else
void enableVirtualTerminalProcessing()
{
}
#endif

static std::optional<unsigned int> getCoordinateCell(const std::string& name)
{
    unsigned int number;
    unsigned int size = 8;
    unsigned int start = 0;
    if (name == "input")
    {
        size = 9;
        start = 1;
    }

    std::print("Enter {1} coordinate between {2} and {0}: ", size, name, start);
    while (!(std::cin >> number) || number < start || number >= size + 1)
    {
        if (std::cin.eof())
        {
            std::println("\nInput stream closed. Exiting.");
            return {};
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::print("Invalid input. Enter {1} between {2} and {0}: ", size, name, start);
    }
    return number;
}

int main()
{
    enableVirtualTerminalProcessing();
    Grid grid;
    grid.generatePuzzle(45);

    while (!grid.win())
    {
        grid.print();
        std::optional<unsigned int> x = getCoordinateCell("x");
        std::optional<unsigned int> y = getCoordinateCell("y");
        std::optional<unsigned int> input = getCoordinateCell("input");

        if (!x || !y || !input)
        {
            std::println("Aborting game.");
            return 0;
        }

        if (!grid.isEditable(x.value(), y.value()))
        {
            std::println("This case [{0};{1}] is not editable", x.value(), y.value());
            continue;
        }

        if (!grid.isCorrect(x.value(), y.value(), input.value()))
        {
            std::println("The value {0} is incorrect", input.value());
            continue;
        }

        grid.setValue(x.value(), y.value(), input.value());
    }
    grid.print();
    std::println("🎉 Congratulations! You solved the Sudoku!");
}
