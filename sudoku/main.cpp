#include <cstdio>

#include "Grid.h"

int main()
{
    printf("Jeu du sudoku\n");
    Grid grid(9);
    grid.print();
    grid.initGrid();
    grid.print();
}
