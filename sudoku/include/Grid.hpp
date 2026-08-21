#ifndef GRID_H
#define GRID_H

#include <random>
#include <vector>

#include "Case.hpp"

struct Grid
{
private:
    std::vector<std::vector<Case>> grid;
    std::vector<std::vector<Case>> solved;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<unsigned int> distX;
    std::uniform_int_distribution<unsigned int> distY;
    [[nodiscard]] bool solveAt(unsigned int x, unsigned int y, bool randomize, unsigned& nbSolutions,
                               unsigned int maxSolutions);
    [[nodiscard]] bool isValidInRow(unsigned int x, unsigned int y, unsigned int value) const;
    [[nodiscard]] bool isValidInColumn(unsigned int x, unsigned int y, unsigned int value) const;
    [[nodiscard]] bool isValidInSquare(unsigned int x, unsigned int y, unsigned int value) const;
    void printSeparator() const noexcept;

public:
    Grid();
    void print() const noexcept;
    void initGrid();
    void generatePuzzle(unsigned int nbCaseEmpty);
    [[nodiscard]] bool isValid(unsigned int x, unsigned int y, unsigned int input) const;
    [[nodiscard]] bool win() const noexcept;
    [[nodiscard]] bool solve();
    [[nodiscard]] bool isEditable(unsigned int x, unsigned int y) const;
    [[nodiscard]] bool isCorrect(unsigned int x, unsigned int y, unsigned int value) const;
    void setValue(unsigned int x, unsigned int y, unsigned int value);
};

#endif //GRID_H
