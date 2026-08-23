#ifndef LANGUAGE_CPP_SUDOKUGRIDTESTACCESS_HPP
#define LANGUAGE_CPP_SUDOKUGRIDTESTACCESS_HPP

#include "Grid.hpp"

struct SudokuGridTestAccess
{
    [[nodiscard]] static unsigned int getValue(const Grid &grid,unsigned int x, unsigned int y) {
        return grid.grid.at(x).at(y).value;
    }

    [[nodiscard]] static NumberOrigin getOrigin(const Grid &grid,unsigned int x, unsigned int y) {
        return grid.grid.at(x).at(y).play;
    }

    [[nodiscard]] static unsigned int getSolvedValue(const Grid &grid,unsigned int x, unsigned int y) {
        return grid.solved.at(x).at(y).value;
    }
};

#endif //LANGUAGE_CPP_SUDOKUGRIDTESTACCESS_HPP
