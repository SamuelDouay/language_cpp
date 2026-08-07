#include "Grille.h"

#include <memory>
#include <random>
#include <vector>
#include <string>

Grille::Grille(int taille)
{
    for (int i = 0; i < taille; i++)
    {
        std::vector<std::unique_ptr<Case>> ligne;

        for (int j = 0; j < taille; j++)
        {
            ligne.push_back(std::make_unique<Case>());
        }
        this->cases.push_back(std::move(ligne));
    }
}

void Grille::print() const
{
    std::printf("Grille du démineur\n  ");

    for (int i = 0; i < this->cases.size(); i++)
    {
        printf("%d ", i);
    }

    std::printf("\n");

    for (int i = 0; i < this->cases.size(); i++)
    {
        std::printf("%i ", i);
        for (const auto& j : this->cases[i])
        {
            if (j->isReveal)
            {
                if (j->isMine)
                {
                    std::printf("* ");
                }
                else if (j->nbMineVoisin > 0)
                {
                    std::printf("%d ", j->nbMineVoisin);
                }
                else
                {
                    std::printf("  ");
                }
            }
            else
            {
                std::printf("# ");
            }
        }
        std::printf("\n");
    }
}

void Grille::placerMines(int nbMines) const
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distX(0, this->cases.size() - 1);
    std::uniform_int_distribution<int> distY(0, this->cases.size() - 1);

    int cpt = 0;
    while (cpt < nbMines)
    {
        int x = distX(gen);
        int y = distY(gen);

        if (!this->cases[x][y]->isMine)
        {
            this->cases[x][y]->isMine = true;
            cpt++;
        }
    }
}

void Grille::calculerMinesVoisines() const
{
    for (int i = 0; i < this->cases.size(); i++)
    {
        for (int j = 0; j < this->cases.size(); j++)
        {
            if (this->cases[i][j]->isMine)
                continue;

            int compteur = 0;

            for (int di = i - 1; di <= i + 1; di++)
            {
                for (int dj = j - 1; dj <= j + 1; dj++)
                {
                    if (isInBorne(di, dj) && !(i == di && j == dj) && this->cases[di][dj]->isMine)
                    {
                        compteur++;
                    }
                }
            }
            this->cases[i][j]->nbMineVoisin = compteur;
        }
    }
}

bool Grille::isInBorne(const int x, const int y) const
{
    return x >= 0 && x < this->cases.size() && y >= 0 && y < this->cases.size();
}

bool Grille::reveal(const int x, const int y) const
{
    this->cases[x][y]->isReveal = true;

    if (this->cases[x][y]->isMine)
    {
        return false;
    }

    if (this->cases[x][y]->nbMineVoisin == 0)
    {
        revealCase(x, y);
    }
    return true;
}

void Grille::revealCase(int x, int y) const
{
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (isInBorne(i, j) && !(i == x && j == y) && !this->cases[i][j]->isReveal)
            {
                this->cases[i][j]->isReveal = true;
                if (this->cases[i][j]->nbMineVoisin == 0 && !this->cases[i][j]->isMine)
                {
                    revealCase(i, j);
                }
            }
        }
    }
}

bool Grille::isWin() const
{
    for (int i = 0; i < this->cases.size(); i++)
    {
        for (int j = 0; j < this->cases.size(); j++)
        {
            if (!this->cases[i][j]->isMine && !this->cases[i][j]->isReveal)
            {
                return false;
            }
        }
    }
    return true;
}
