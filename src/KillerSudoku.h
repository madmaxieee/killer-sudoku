#pragma once

#include <iostream>
#include <utility>
#include <random>
#include <vector>
#include <unordered_set>

// each individual cell on the board
typedef std::pair<int, int> Cell;

// cell hash
struct CellHash
{
    std::size_t operator()(const Cell &c) const
    {
        return std::hash<int>()(c.first) ^ std::hash<int>()(c.second);
    }
};

// cells in a box can't have the same value
typedef std::unordered_set<Cell, CellHash> Box;

// a group of cells that must add up to a certain value
struct Cage
{
    int sum;
    std::vector<Cell> cells;
};

class KillerSudoku
{
public:
    KillerSudoku(int size);
    ~KillerSudoku();
    void newGame();
    void printSolution();
    void printCages();

    void generateNewSolution();
    void generateCages();

    bool checkSolution(std::vector<std::vector<int>> &solution);

    // getters
    std::vector<Box> &getBoxes();
    std::vector<std::vector<int>> &getSolution();
    std::vector<Cage> &getCages();

private:
    int size;
    std::vector<Box> boxes;
    std::vector<std::vector<int>> solution;
    std::vector<Cage> cages;

    std::default_random_engine rng;

    void initBoxes();
    bool fillCells(int row, int col);
    void generateCage(Cage &cage, std::vector<std::vector<bool>> &covered);
};
