#ifndef GRILLE_H
#define GRILLE_H

#include <memory>
#include <vector>
#include "Case.h"

struct Grille
{
private:
    std::vector<std::vector<Case>> cases;
    void revealCase( int i,  int j) ;

public:
    Grille(unsigned int size);
    void print() const;
    void placerMines(unsigned int nbMines) ;
    void calculerMinesVoisines() ;
    bool reveal( int x, int j) ;
    bool isWin() const;
};

#endif //GRILLE_H
