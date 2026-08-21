# C++ Learning Projects

This repository groups several projects implemented in modern C++ (C++23) for practicing object-oriented programming,
genericity, data structures and algorithms, error handling, concurrency, and testing best practices.

## Contents

### 1. Minesweeper

A console-based Minesweeper game.

- **Features**:
    - Random grid generation with mines.
    - Cell revealing (with recursive or iterative cascade).
    - Flag marking.
    - Win/loss detection.
- **Key concepts**:
    - OOP, classes, inheritance, polymorphism.
    - Use of `std::unique_ptr`, `std::vector`.
    - RAII, `std::move`, `std::random`.
- **Compilation**: see instructions below.

---

### 2. Sudoku (solver and generator)

A complete Sudoku program:

- **Solver**: solves any partial grid via backtracking.
- **Puzzle generator**: creates a random grid with a unique solution (up to ~50 empty cells).
- **Player mode**: value input, solution verification, colors.
- **Key concepts**:
    - Recursive backtracking.
    - State management, grid copy/restoration.
    - Validation algorithms (rows, columns, blocks).
    - `std::shuffle`, random generation.
    - ANSI sequences for colored output, natively enabled on Linux/macOS and via the Windows Virtual Terminal API on
      Windows, with conditional code (`#ifdef _WIN32`) for true cross‑platform compilation.
- **Compilation**: see instructions below.

---

### 3. MiniVector<T> & DynamicBitset (in progress)

A generic dynamic container and a dynamic bitset.

- **Goals**:
    - Implement a `MiniVector<T>` similar to `std::vector` (dynamic allocation, rule of 5, simple iterators).
    - Build a `DynamicBitset` based on `MiniVector<unsigned char>` with binary operations (`set`, `reset`, `test`,
      `flip`).
- **Key concepts**:
    - Class templates.
    - Memory management (`std::unique_ptr`).
    - Operator overloading.
    - Bitwise operations.
- **Status**: under development.

---

### 4. Generic Binary Search Tree (upcoming)

A generic BST (Binary Search Tree) with insertion, search, deletion, and traversals.

- **Planned features**:
    - `BinarySearchTree<T>` parameterised by type.
    - In‑order, pre‑order, post‑order traversals.
    - Simple iterators.
    - Use of `std::unique_ptr` for node management.
- **Key concepts**:
    - Class and function templates.
    - Recursion.
    - Smart pointers.
    - Customisable comparators.
- **Status**: to be started.

---

### 5. Arithmetic Expression Evaluator (upcoming)

A parser and evaluator for mathematical expressions in infix notation (e.g., `"3 + 4 * (2 - 1)"` → `7`).

- **Planned features**:
    - Lexical analysis: tokenise a string into tokens (numbers, operators, parentheses).
    - Recursive‑descent parser respecting operator precedence (`*`/`/` before `+`/`-`) and parentheses.
    - Construction of an Abstract Syntax Tree (AST) representing the expression.
    - Recursive evaluation of the AST.
    - Error handling: invalid syntax, division by zero, mismatched parentheses.
- **Key concepts**:
    - Recursion and syntax trees.
    - Separation of lexer / parser / evaluator.
    - `enum class` for token types.
    - Proper error handling (exceptions, or `std::optional`/`std::expected`).
- **Status**: to be started.

---

### 6. Mini JSON Parser / JSON API (upcoming)

A minimal JSON parser and an API for manipulating JSON data.

- **Planned features**:
    - Lexical and syntactic analysis of a JSON file.
    - Building a tree of values (`JsonValue`).
    - Field access, iteration over arrays.
    - Serialisation (back to JSON text).
- **Key concepts**:
    - Variadic templates.
    - `std::variant`, `std::optional`, `std::vector`, `std::map`.
    - Recursion over structures.
    - Type traits and SFINAE.
- **Status**: to be started.

---

### 7. Concurrency Project (upcoming)

A simple thread pool and/or a producer‑consumer example.

- **Planned features**:
    - `ThreadPool`: task submission (`std::function`, `std::future`), thread‑safe queue.
    - Producer‑consumer example with multiple producer and consumer threads.
    - Proper synchronisation (no race conditions, no deadlocks).
- **Key concepts**:
    - `std::thread`, `std::mutex`, `std::lock_guard`, `std::unique_lock`.
    - `std::condition_variable`, `std::atomic`.
    - `std::future` / `std::promise`.
    - Race conditions, deadlocks, and how to avoid them.
- **Status**: to be started.

---

## Automated Testing

Goal: replace manual validation (compile + run + visual check) with a versioned, reproducible, and continuously
executable test suite.

- **Framework**: Catch2 (header‑only, lightweight, good CMake integration) — to be confirmed against GoogleTest
  depending on needs.
- **Initial scope** (already completed projects, tests added retroactively):
    - **Minesweeper**: mine placement, neighbour mine count, cascade reveal, win/loss detection.
    - **Sudoku**: row/column/block validation, backtracking solve, uniqueness of generated solution, consistency of
      `generatePuzzle` (number of holes, state restoration).
- **Future scope**: every new project (MiniVector, DynamicBitset, BST, expression evaluator, JSON, concurrency) is
  delivered with its own test suite from the start, rather than added afterwards.
- **Status**: to be integrated.

---

## Prerequisites

- **C++23 compiler**: GCC ≥ 13, Clang ≥ 17, or MSVC ≥ 19.30.
- **CMake ≥ 3.20** (recommended) or a simple Makefile.
- **Operating system**: Windows 10/11, Linux, macOS.
- For coloured output on Windows, the program automatically enables virtual terminal mode; on Linux/macOS, ANSI codes
  work natively.
- For the concurrency project, a properly linked threads library (`-lpthread` on Linux/macOS depending on compiler;
  usually automatic with CMake via `Threads::Threads`).
- For automated tests, Catch2 (retrievable via CMake `FetchContent`, or a package manager like vcpkg / Conan).

---

## Clone the Repository

Don't forget to pull the submodules with the repository using --recursive:

```bash
git clone --recursive https://github.com/SamuelDouay/language_cpp.git
```

or if you have already cloned the repository:

```bash
git submodule init
git submodule update
```

To build the project you will need a C++23 compliant compiler.

---

## Compilation and Execution

Each project is independent and has its own folder.

### With CMake (recommandé)

```bash
# At the root of the repository
cmake -S . -B build
cmake --build build

# Run a project (e.g., Sudoku)
./build/sudoku

# Run the test suite (once Catch2 is integrated)
ctest --test-dir build
```

---

### Manual compilation (example for Sudoku)

```bash
cd sudoku
g++ -std=c++23 -Wall -Wextra -O2 src/*.cpp -o sudoku
./sudoku
```