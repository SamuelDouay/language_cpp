#include <cmath>
#include <format>
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

bool estPremier(int i)
{
    bool premier = true;


    if (i < 2)
        premier = false;

    for (int j = 2; j < i - 1; j++)
    {
        if (i % j == 0)
        {
            premier = false;
        }
    }
    return premier;
}

int pgcd(int a, int b)
{
    if (b == 0)
        return a;
    return pgcd(b, a % b);
}

void swap(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

void tableau()
{
    int tab[5] = {4, 8, 15, 16, 23};
    int nb = tab[0], somme = 0;

    for (int i : tab)
    {
        if (i > nb)
        {
            nb = i;
        }
        somme += i;
    }
    printf("plus grand nombre = %d, somme = %d", nb, somme);
}

void vector_ex()
{
    vector<int> tab;

    printf("Combien d'élèments tu veux insérer  ?\n");

    int n;

    cin >> n;

    for (int i = 0; i < n; i++)
    {
        int num;
        printf("Entre ton numero %d\n", i);

        cin >> num;

        tab.push_back(num);
    }

    printf("tableau de taille %zu\n", tab.size());
    printf("element du tableau : [");
    for (int i : tab)
    {
        printf("%d ", i);
    }
    printf("]\n");
}

void pointeur()
{
    int x = 25;
    int* p = &x;

    printf("value x = %d\n", x);
    printf("address x = %p\n", &x);
    printf("value p = %p\n", p);
    printf("address p = %p\n", &p);

    *p = 100;


    printf("value x = %d\n", x);
    printf("address x = %p\n", &x);
    printf("value p = %p\n", p);
    printf("address p = %p\n", &p);
}

void echangerPtr(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void reference_pointer()
{
    int a = 10, k = 20;
    int *b = &a, *c = &k;

    printf("b = %d\n", *b);
    printf("c = %d\n", *c);

    echangerPtr(b, c);

    printf("b = %d\n", *b);
    printf("c = %d\n", *c);
}

void pointer_tab()
{
    int tab[5] = {10, 20, 30, 40, 50};
    int* p = tab;

    for (int i = 0; i < 5; i++)
    {
        printf("tab[%d] = %d\n", i, *p);
        p++;
    }
}


class Rectangle
{
private:
    double largeur;
    double hauteur;

public:
    Rectangle(const double l, const double h)
    {
        largeur = l;
        hauteur = h;
    }

    double area()
    {
        return largeur * hauteur;
    }

    double perimetre()
    {
        return 2 * (largeur + hauteur);
    }

    double getLargeur()
    {
        return largeur;
    }

    double getHauteur()
    {
        return hauteur;
    }

    void redemensioner(double fact)
    {
        this->largeur = fact * largeur;
        this->hauteur = fact * hauteur;
    }
};

void poo()
{
    Rectangle rect(20.5, 45.6);

    printf("rectangle largeur: %lf\n", rect.getLargeur());
    printf("rectangle hauteur: %lf\n", rect.getHauteur());
    printf("rectangle area : %lf\n", rect.area());
    printf("rectangle perimetre : %lf\n", rect.perimetre());

    rect.redemensioner(0.5);

    printf("rectangle largeur: %lf\n", rect.getLargeur());
    printf("rectangle hauteur: %lf\n", rect.getHauteur());
    printf("rectangle area : %lf\n", rect.area());
    printf("rectangle perimetre : %lf\n", rect.perimetre());
}

class Vehicules
{
protected:
    double vitesseMax;

public:
    Vehicules(const double vitesseMax)
    {
        this->vitesseMax = vitesseMax;
    }

    void afficherVitesseMax()
    {
        printf("vitesseMax = %lf\n", vitesseMax);
    }

    virtual void afficherType()
    {
        printf("Vehicule de type générique \n");
    }
};

class Voiture : public Vehicules
{
private:
    int nombrePorte;

public:
    Voiture(const double vitesseMax, const int nombrePorte) : Vehicules(vitesseMax)
    {
        this->nombrePorte = nombrePorte;
    }

    void afficherInfos()
    {
        printf("nombre porte = %d\n", nombrePorte);
        this->afficherVitesseMax();
    }

    void afficherType()
    {
        printf("C'est une voiture\n");
    }
};

class Moto : public Vehicules
{
public:
    Moto(const double vitesseMax) : Vehicules(vitesseMax)
    {
    }

    void afficherType()
    {
        printf("C'est une moto\n");
    }
};

void poo_heritage()
{
    Voiture voiture(120.5, 5);

    voiture.afficherInfos();
    voiture.afficherType();

    Moto moto(120.5);
    moto.afficherType();

    Vehicules* v = new Voiture(120.5, 5);
    v->afficherType();

    vector<Vehicules*> vehicules;
    vehicules.push_back(new Voiture(120.5, 5));
    vehicules.push_back(new Moto(180.0));

    for (Vehicules* v : vehicules)
    {
        v->afficherType(); // affiche le bon message selon le type réel !
    }
}

namespace
{
    struct Point
    {
        double x;
        double y;

        double distanceOrigine() const
        {
            return sqrt(x * x + y * y);
        }

        double distanceAutrePoint(const Point point) const
        {
            return sqrt(pow(x - point.x, 2) + pow(y - point.y, 2));
        }
    };
}

int main()
{
    //Point point(4.0, 5.0);
    //Point point2(3.0, 7.0);

    //printf("point 1 x: %f, y: %f\n", point.x, point.y);
    //printf("point 2 x: %f, y: %f\n", point2.x, point2.y);

    //printf("distance depuis l'origine point 1 : %f, point 2 : %f\n", point.distanceOrigine(), point2.distanceOrigine());
    //printf("Distance entre les deux points : %f", point.distanceAutrePoint(point2));

    return 0;
}
