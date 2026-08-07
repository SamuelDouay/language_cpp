#ifndef GRILLE_H
#define GRILLE_H

#include <memory>
#include <vector>
#include "Case.h"

struct Grille
{
private:
    std::vector<std::vector<std::unique_ptr<Case>>> cases;
    bool isInBorne(int x, int y) const;
    void revealCase(int x, int y) const;

public:
    Grille(int taille);
    void print() const;
    void placerMines(int nbMines) const;
    void calculerMinesVoisines() const;
    bool reveal(int x, int y) const;
    bool isWin() const;
};

#endif //GRILLE_H
