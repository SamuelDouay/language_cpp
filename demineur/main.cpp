#include <iostream>
#include <print>

#include "Grille.h"

int main()
{
    constexpr unsigned int size = 10;
    constexpr unsigned int nbMines = 15;
    Grille grille(size);
    grille.placerMines(nbMines);
    grille.calculerMinesVoisines();

    bool gameOver = false;
    bool gagne = false;

    while (!gameOver)
    {
        grille.print();
        int x, y;

        do
        {
            std::print("Entrez un nombre x entre 0 et {0}: ", size - 1);
            std::cin >> x;
        }
        while (x < 0 || x >= size);

        do
        {
            std::print("Entrez un nombre y entre 0 et {0}: ", size - 1);
            std::cin >> y;
        }
        while (y < 0 || y >= size);

        if (!grille.reveal(x, y))
        {
            gameOver = true; // mine touchée
        }
        else if (grille.isWin())
        {
            gameOver = true;
            gagne = true;
        }
    }

    grille.print();
    
    if (gagne)
        std::println("Bravo, tu as gagné !");
    else
        std::println("Boom, perdu...");

    return 0;
}
