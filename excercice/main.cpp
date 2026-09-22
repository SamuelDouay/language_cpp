#include <cmath>
#include <format>
#include <iostream>
#include <print>
#include <vector>
using namespace std;

static bool estPremier(int i)
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

static int pgcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return pgcd(b, a % b);
}

static void swap(int& a, int& b)
{
    int const tmp = a;
    a = b;
    b = tmp;
}

static void tableau()
{
    int const tab[5] = {4, 8, 15, 16, 23};
    int nb = tab[0];
    int somme = 0;

    for (int i : tab)
    {
        if (i > nb)
        {
            nb = i;
        }
        somme += i;
    }
    std::print("plus grand nombre = {}, somme = {}", nb, somme);
}

static void vector_ex()
{
    vector<int> tab;

    std::println("Combien d'élèments tu veux insérer  ?");

    int n = 0;

    cin >> n;

    for (int i = 0; i < n; i++)
    {
        int num = 0;
        std::println("Entre ton numero {}", i);

        cin >> num;

        tab.push_back(num);
    }

    std::println("tableau de taille {}", tab.size());
    std::print("element du tableau : [");
    for (int i : tab)
    {
        std::print("{} ", i);
    }
    std::println("]");
}

static void pointeur()
{
    int x = 25;
    int* p = &x;

    std::println("value x = {}", x);
    std::println("address x = {}", static_cast<const void*>(&x));
    std::println("value p = {}", static_cast<const void*>(p));
    std::println("address p = {}", static_cast<const void*>(&p));

    *p = 100;


    std::println("value x = {}", x);
    std::println("address x = {}", static_cast<const void*>(&x));
    std::println("value p = {}", static_cast<const void*>(p));
    std::println("address p = {}", static_cast<const void*>(&p));
}

static void echangerPtr(int* a, int* b)
{
    int const temp = *a;
    *a = *b;
    *b = temp;
}

static void reference_pointer()
{
    int a = 10;
    int k = 20;
    int* b = &a;
    int* c = &k;

    std::println("b = {}", *b);
    std::println("c = {}", *c);

    echangerPtr(b, c);

    std::println("b = {}", *b);
    std::println("c = {}", *c);
}

static void pointer_tab()
{
    int const tab[5] = {10, 20, 30, 40, 50};
    int const* p = tab;

    for (int i = 0; i < 5; i++)
    {
        std::println("tab[{}] = {}", i, *p);
        p++;
    }
}


namespace
{
    class Rectangle
    {
    private:
        double largeur;
        double hauteur;

    public:
        Rectangle(const double l, const double h) : largeur(l), hauteur(h)
        {
        }

        [[nodiscard]] double area() const
        {
            return largeur * hauteur;
        }

        [[nodiscard]] double perimetre() const
        {
            return 2 * (largeur + hauteur);
        }

        [[nodiscard]] double getLargeur() const
        {
            return largeur;
        }

        [[nodiscard]] double getHauteur() const
        {
            return hauteur;
        }

        void redemensioner(const double fact)
        {
            this->largeur = fact * largeur;
            this->hauteur = fact * hauteur;
        }
    };
}

static void poo()
{
    Rectangle rect(20.5, 45.6);

    std::println("rectangle largeur: {:f}", rect.getLargeur());
    std::println("rectangle hauteur: {:f}", rect.getHauteur());
    std::println("rectangle area : {:f}", rect.area());
    std::println("rectangle perimetre : {:f}", rect.perimetre());

    rect.redemensioner(0.5);

    std::println("rectangle largeur: {:f}", rect.getLargeur());
    std::println("rectangle hauteur: {:f}", rect.getHauteur());
    std::println("rectangle area : {:f}", rect.area());
    std::println("rectangle perimetre : {:f}", rect.perimetre());
}

class Vehicules
{
protected:
    double vitesseMax;

public:
    explicit Vehicules(const double vitesseMax) : vitesseMax(vitesseMax)
    {
    }

    void afficherVitesseMax()
    {
        std::println("vitesseMax = {:f}", vitesseMax);
    }

    virtual void afficherType()
    {
        std::println("Vehicule de type générique ");
    }
};

class Voiture : public Vehicules
{
private:
    int nombrePorte;

public:
    Voiture(const double vitesseMax, const int nombrePorte) : Vehicules(vitesseMax), nombrePorte(nombrePorte)
    {
    }

    void afficherInfos()
    {
        std::println("nombre porte = {}", nombrePorte);
        this->afficherVitesseMax();
    }

    void afficherType() override
    {
        std::println("C'est une voiture");
    }
};

class Moto : public Vehicules
{
public:
    explicit Moto(const double vitesseMax) : Vehicules(vitesseMax)
    {
    }

    void afficherType() override
    {
        std::println("C'est une moto");
    }
};

static void poo_heritage()
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

        [[nodiscard]] double distanceOrigine() const
        {
            return sqrt(x * x + y * y);
        }

        [[nodiscard]] double distanceAutrePoint(const Point point) const
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
