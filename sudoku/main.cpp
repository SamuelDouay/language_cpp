#include <print>

#include "Grid.hpp"

int main()
{
    Grid grid;

    /* grid.print();

     grid.solve();
     grid.print(); */

    //grid.initGrid();
    // grid.print();

    grid.generatePuzzle(30);
    grid.print();
}
