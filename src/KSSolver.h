#pragma once

#include <tuple>
#include <vector>
#include "KillerSudoku.h"
#include "PBSatSolver.h"

typedef std::tuple<std::vector<Lit>, std::vector<int>, int> PBC;

class KSSolver
{
private:
    int size;
    bool result;
    PBSatSolver _solver;
    std::vector<Cage> cages;
    std::vector<std::vector<std::vector<Var>>> variables;

    std::vector<std::vector<int>> solution;
    std::vector<std::vector<Cage>> cell2cage;

public:
    KSSolver(int size);
    ~KSSolver();
    void createConstraints(const std::vector<Cage> &cages);
    void solve(const std::vector<Cage> &cages, bool verbose = true);
    void reportResult();
    void printStats();

    void bruteForceSolve(const std::vector<Cage> &cages, bool verbose = true);

private:
    void createVars();

    void createEncodingConstraints();
    void createRowConstraints();
    void createColumnConstraints();
    void createBoxConstraints();
    void createCageConstraints();

    bool bruteForce();
    std::pair<int, int> getFirstEmptyCell();
};
