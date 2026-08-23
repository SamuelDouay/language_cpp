#include <catch2/catch_test_macros.hpp>

#include "Cell.hpp"
#include "Grid.hpp"
#include "SudokuGridTestAccess.hpp"

TEST_CASE("Grid constructor initializes empty 9x9", "[grid]")
{
    Grid g;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            REQUIRE(SudokuGridTestAccess::getValue(g, i, j) == 0);
            REQUIRE(SudokuGridTestAccess::getOrigin(g,i, j) == NumberOrigin::FIXED);
        }
    }
}

TEST_CASE("isValid detects row conflict", "[grid]")
{
    Grid g;
    g.setValue(0, 0, 5);
    g.setValue(0, 1, 5);
    REQUIRE_FALSE(g.isValid(0, 0, 5));
}

TEST_CASE("isValid detects column conflict", "[grid]")
{
    Grid g;
    g.setValue(1, 0, 7);
    REQUIRE_FALSE(g.isValid(0, 0, 7));
}

TEST_CASE("isValid detects square conflict", "[grid]")
{
    Grid g;
    g.setValue(0, 0, 3);
    REQUIRE_FALSE(g.isValid(0, 1, 3));
}

TEST_CASE("isValid returns true for non-conflicting value", "[grid]")
{
    Grid g;
    REQUIRE(g.isValid(0, 0, 1));
}

TEST_CASE("solve solves empty grid", "[grid]")
{
    Grid g;
    REQUIRE(g.solve());

    REQUIRE(g.win());

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            REQUIRE(SudokuGridTestAccess::getValue(g,i, j) > 0);
}

TEST_CASE("win returns false on incomplete grid", "[grid]")
{
    Grid g;
    REQUIRE_FALSE(g.win());
    g.setValue(0, 0, 1);
    REQUIRE_FALSE(g.win());
}

TEST_CASE("initGrid generates full valid grid", "[grid]")
{
    Grid g;
    g.initGrid();

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            REQUIRE(SudokuGridTestAccess::getValue(g,i, j) > 0);

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            REQUIRE(SudokuGridTestAccess::getSolvedValue(g,i, j) == SudokuGridTestAccess::getValue(g,i, j));
}

TEST_CASE("generatePuzzle creates specified number of empty cells", "[grid]")
{
    Grid g;
    const unsigned int nbEmpty = 40;
    g.generatePuzzle(nbEmpty);
    int emptyCount = 0;
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (SudokuGridTestAccess::getValue(g,i, j) == 0)
            {
                ++emptyCount;
                REQUIRE(SudokuGridTestAccess::getOrigin(g,i, j) == NumberOrigin::PLAYER);
            }
            else
            {
                REQUIRE(SudokuGridTestAccess::getOrigin(g,i, j) == NumberOrigin::FIXED);
            }
        }
    }
    REQUIRE(emptyCount == nbEmpty);
}

TEST_CASE("isCorrect works with solved grid", "[grid]")
{
    Grid g;
    g.initGrid();

    int testX = 0, testY = 0;
    unsigned int correctVal = SudokuGridTestAccess::getSolvedValue(g,testX, testY);
    REQUIRE(g.isCorrect(testX, testY, correctVal));
    // Incorrect value
    unsigned int wrongVal = (correctVal % 9) + 1;
    REQUIRE_FALSE(g.isCorrect(testX, testY, wrongVal));
}

TEST_CASE("isEditable returns true only for player cells", "[grid]")
{
    Grid g;
    g.generatePuzzle(45);

    bool foundPlayer = false, foundFixed = false;
    for (int i = 0; i < 9 && !(foundPlayer && foundFixed); ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (SudokuGridTestAccess::getOrigin(g,i, j) == NumberOrigin::PLAYER)
                foundPlayer = true;
            else
                foundFixed = true;
        }
    }
    REQUIRE(foundPlayer);
    REQUIRE(foundFixed);

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (SudokuGridTestAccess::getOrigin(g,i, j) == NumberOrigin::PLAYER)
                REQUIRE(g.isEditable(i, j));
}

TEST_CASE("setValue modifies cell", "[grid]")
{
    Grid g;
    g.generatePuzzle(40);

    int px = -1, py = -1;
    for (int i = 0; i < 9 && px == -1; ++i)
        for (int j = 0; j < 9; ++j)
            if (SudokuGridTestAccess::getOrigin(g,i, j) == NumberOrigin::PLAYER)
            {
                px = i; py = j;
                break;
            }
    REQUIRE(px != -1);
    unsigned int newVal = 5;
    g.setValue(px, py, newVal);
    REQUIRE(SudokuGridTestAccess::getValue(g,px, py) == newVal);
}