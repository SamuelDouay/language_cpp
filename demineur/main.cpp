#include <iostream>

#include "Grille.h"

int main()
{
    int taille = 10, nbMines = 15;
    Grille grille(taille);
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
            printf("Entrez un nombre x entre 0 et %d: ", taille - 1);
            std::cin >> x;
        }
        while (x < 0 || x >= taille);

        do
        {
            printf("Entrez un nombre y entre 0 et %d: ", taille - 1);
            std::cin >> y;
        }
        while (y < 0 || y >= taille);

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
        printf("Bravo, tu as gagné !\n");
    else
        printf("Boom, perdu...\n");

    return 0;
}
