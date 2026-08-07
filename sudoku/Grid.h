#ifndef GRID_H
#define GRID_H
#include <memory>
#include <vector>

#include "./sudoku/Case.h"

struct Grid
{
    private:
        std::vector<std::vector<std::unique_ptr<Case>>> grid;

    public:
        Grid(int taille);
        void print();
        void initGrid();
};

#endif //GRID_H
