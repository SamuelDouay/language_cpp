#include "Grille.h"

#include <random>
#include <vector>
#include <print>
#include <ranges>

Grille::Grille(unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        std::vector<Case> ligne;

        for (unsigned int j = 0; j < size; j++)
        {
            ligne.emplace_back(); // créer l'objet directement dans le vector
        }
        cases.push_back(ligne);
    }
}

void Grille::print() const
{
    std::print("Grille du démineur\n  ");

    for (unsigned int i = 0; i < cases.size(); i++)
    {
        std::print("{0} ", i);
    }

    std::println();

    for ( const auto& [i, ligne] : cases  | std::views::enumerate)
    {
        std::print("{0} ", i);
        for (const auto& j : ligne)
        {
            if (j.isReveal)
            {
                if (j.isMine)
                {
                    std::print("* ");
                }
                else if (j.nbMineVoisin > 0)
                {
                    std::print("{0} ", j.nbMineVoisin);
                }
                else
                {
                    std::print("  ");
                }
            }
            else
            {
                std::print("# ");
            }
        }
        std::println();
    }
}

void Grille::placerMines(const unsigned int nbMines)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> distX(0, cases.size() - 1);
    std::uniform_int_distribution<unsigned int> distY(0, cases.size() - 1);

    unsigned int  cpt = 0;
    while (cpt < nbMines)
    {
        const unsigned int x = distX(gen);
        const unsigned int y = distY(gen);

        if (!cases.at(x).at(y).isMine)
        {
            cases.at(x).at(y).isMine = true;
            cpt++;
        }
    }
}

void Grille::calculerMinesVoisines()
{

    for (int i = 0; i < cases.size(); i++)
    {
        for (int j = 0; j < cases.size(); j++)
        {
            if (cases.at(i).at(j).isMine || cases.at(i).at(j).nbMineVoisin != 0)
                continue;

            int compteur = 0;

            for (int di = std::max(0, i - 1); di < std::min(i + 2, static_cast<int>(cases.size())); di++)
            {

                for (int dj = std::max(0, j - 1); dj <  std::min( j + 2, static_cast<int>(cases.size())); dj++)
                {
                    if (di == i && dj == j)
                    {
                        continue;
                    }
                    if (cases.at(di).at(dj).isMine)
                    {
                        compteur++;
                    }
                }
            }
            cases.at(i).at(j).nbMineVoisin = compteur;
        }
    }
}

bool Grille::reveal(const  int x, const  int y)
{
    cases.at(x).at(y).isReveal = true;

    if (cases.at(x).at(y).isMine)
    {
        return false;
    }

    if (cases.at(x).at(y).nbMineVoisin == 0)
    {
        revealCase(x, y);
    }
    return true;
}

void Grille::revealCase(const int i,const  int j)
{
    for (int di = std::max(0, i - 1); di < std::min(i + 2, static_cast<int>(cases.size())); di++)
    {
        for (int dj = std::max(0, j - 1); dj <  std::min( j + 2, static_cast<int>(cases.size())); dj++)
        {
            if (di == i && dj == j)
            {
                continue;
            }
            if (!cases.at(di).at(dj).isReveal)
            {
                cases.at(di).at(dj).isReveal = true;
                if (cases.at(di).at(dj).nbMineVoisin == 0 && !cases.at(di).at(dj).isMine)
                {
                    revealCase(di, dj);
                }
            }
        }
    }
}

bool Grille::isWin() const
{
    for (auto & i : cases)
    {
        for (auto & j : i)
        {
            if (!j.isMine && !j.isReveal)
            {
                return false;
            }
        }
    }
    return true;
}
