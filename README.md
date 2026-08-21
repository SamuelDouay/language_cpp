# Projets C++ d'apprentissage

Ce dépôt regroupe plusieurs projets réalisés en C++ moderne (C++23) dans le but de pratiquer la programmation orientée
objet, la généricité, les structures de données et les algorithmes, la gestion d'erreurs, la concurrence, et les bonnes
pratiques de test.

## Contenu

### 1. Démineur

Un jeu de démineur en console.

- **Fonctionnalités** :
    - Génération aléatoire de la grille avec mines.
    - Révélation de cases (avec cascade récursive ou itérative).
    - Marquage de drapeaux.
    - Détection de victoire/défaite.
- **Concepts clés** :
    - POO, classes, héritage, polymorphisme.
    - Utilisation de `std::unique_ptr`, `std::vector`.
    - RAII, `std::move`, `std::random`.
- **Compilation** : voir instructions plus bas.

---

### 2. Sudoku (solveur et générateur)

Un programme complet de Sudoku :

- **Solveur** : résout n'importe quelle grille partielle via backtracking.
- **Générateur de puzzle** : crée une grille aléatoire avec solution unique (jusqu'à ~50 cases vides).
- **Mode joueur** : saisie de valeurs, vérification de la solution, couleurs.
- **Concepts clés** :
    - Backtracking récursif.
    - Gestion d'état, copie/restauration de grille.
    - Algorithmes de validation (lignes, colonnes, blocs).
    - `std::shuffle`, génération aléatoire.
    - Séquences ANSI pour l'affichage coloré, activées nativement sous Linux/macOS et via l'API Windows (mode terminal
      virtuel) sous Windows, avec code conditionnel (`#ifdef _WIN32`) pour une compilation multiplateforme réelle.
- **Compilation** : voir instructions plus bas.

---

### 3. MiniVector<T> & DynamicBitset (en cours)

Un conteneur dynamique générique et un bitset dynamique.

- **Objectifs** :
    - Implémenter un `MiniVector<T>` similaire à `std::vector` (allocation dynamique, rule of 5, itérateurs simples).
    - Construire un `DynamicBitset` basé sur `MiniVector<unsigned char>` avec opérations binaires (`set`, `reset`,
      `test`, `flip`).
- **Concepts clés** :
    - Templates de classes.
    - Gestion de mémoire (`std::unique_ptr`).
    - Surcharge d'opérateurs.
    - Opérations bit à bit.
- **Statut** : en cours de développement.

---

### 4. Arbre binaire de recherche générique (à venir)

Un BST (Binary Search Tree) générique avec insertion, recherche, suppression et parcours.

- **Fonctionnalités prévues** :
    - `BinarySearchTree<T>` paramétrable par type.
    - Parcours infixe, préfixe, postfixe.
    - Itérateurs simples.
    - Utilisation de `std::unique_ptr` pour la gestion des nœuds.
- **Concepts clés** :
    - Templates de classes et de fonctions.
    - Récursivité.
    - Pointeurs intelligents.
    - Comparateurs paramétrables.
- **Statut** : à commencer.

---

### 5. Évaluateur d'expressions arithmétiques (à venir)

Un parseur et évaluateur d'expressions mathématiques en notation infixe (ex : `"3 + 4 * (2 - 1)"` → `7`).

- **Fonctionnalités prévues** :
    - Analyse lexicale : découpage d'une chaîne en tokens (nombres, opérateurs, parenthèses).
    - Parseur récursif descendant respectant la priorité des opérateurs (`*`/`/` avant `+`/`-`) et les parenthèses.
    - Construction d'un arbre syntaxique (AST) représentant l'expression.
    - Évaluation récursive de l'AST.
    - Gestion des erreurs : syntaxe invalide, division par zéro, parenthèses non fermées.
- **Concepts clés** :
    - Récursivité et arbres syntaxiques.
    - Séparation lexer / parser / évaluateur.
    - `enum class` pour les types de tokens.
    - Gestion d'erreurs propre (exceptions, ou `std::optional`/`std::expected`).
- **Statut** : à commencer.

---

### 6. Mini parseur JSON / API JSON (à venir)

Un parseur JSON minimal et une API pour manipuler des données JSON.

- **Fonctionnalités prévues** :
    - Analyse lexicale et syntaxique d'un fichier JSON.
    - Construction d'un arbre de valeurs (`JsonValue`).
    - Accès aux champs, itération sur les tableaux.
    - Sérialisation (retour en texte JSON).
- **Concepts clés** :
    - Templates variadiques.
    - `std::variant`, `std::optional`, `std::vector`, `std::map`.
    - Récursivité sur les structures.
    - Traits de types et SFINAE.
- **Statut** : à commencer.

---

### 7. Projet de concurrence (à venir)

Un pool de threads simple et/ou un exemple producteur-consommateur.

- **Fonctionnalités prévues** :
    - `ThreadPool` : soumission de tâches (`std::function`, `std::future`), file d'attente thread-safe.
    - Exemple producteur-consommateur avec plusieurs threads producteurs et consommateurs.
    - Synchronisation correcte (pas de race condition, pas de deadlock).
- **Concepts clés** :
    - `std::thread`, `std::mutex`, `std::lock_guard`, `std::unique_lock`.
    - `std::condition_variable`, `std::atomic`.
    - `std::future` / `std::promise`.
    - Race conditions, deadlocks, et comment les éviter.
- **Statut** : à commencer.

---

## Tests automatisés

Objectif : remplacer la validation manuelle (compilation + exécution + vérification visuelle) par une suite de tests
versionnée, reproductible, et exécutable en continu.

- **Framework** : Catch2 (header-only, léger, bonne intégration CMake) — à confirmer par rapport à GoogleTest selon les
  besoins.
- **Portée initiale** (projets déjà réalisés, tests ajoutés rétroactivement) :
    - **Démineur** : placement des mines, calcul des mines voisines, cascade de révélation, détection victoire/défaite.
    - **Sudoku** : validation ligne/colonne/bloc, résolution par backtracking, unicité de la solution générée, cohérence
      de `generatePuzzle` (nombre de trous, restauration d'état).
- **Portée future** : chaque nouveau projet (MiniVector, DynamicBitset, BST, évaluateur d'expressions, JSON,
  concurrence) est livré avec sa suite de tests dès le départ, plutôt qu'ajoutée après coup.
- **Statut** : à intégrer.

---

## Prérequis

- **Compilateur C++23** : GCC ≥ 13, Clang ≥ 17 ou MSVC ≥ 19.30.
- **CMake ≥ 3.20** (recommandé) ou un simple Makefile.
- **Système d'exploitation** : Windows 10/11, Linux, macOS.
- Pour l'affichage coloré sous Windows, le programme active automatiquement le mode terminal virtuel ; sous Linux/macOS,
  les codes ANSI fonctionnent nativement.
- Pour le projet de concurrence, une bibliothèque threads correctement liée (`-lpthread` sous Linux/macOS selon le
  compilateur ; généralement automatique avec CMake via `Threads::Threads`).
- Pour les tests automatisés, Catch2 (récupérable via CMake `FetchContent`, ou un gestionnaire de paquets comme vcpkg /
  Conan).

---

## Compilation et exécution

Chaque projet est indépendant et possède son propre dossier.

### Avec CMake (recommandé)

```bash
# À la racine du dépôt
cmake -S . -B build
cmake --build build

# Exécuter un projet (exemple pour le Sudoku)
./build/sudoku

# Exécuter la suite de tests (une fois Catch2 intégré)
ctest --test-dir build
```

---

### Compilation manuelle (exemple pour le Sudoku)

```bash
cd sudoku
g++ -std=c++23 -Wall -Wextra -O2 src/*.cpp -o sudoku
./sudoku
```