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
    std::vector<Box> boxes;
    std::vector<std::vector<std::vector<Var>>> variables;

public:
    KSSolver(int size);
    ~KSSolver();
    void createConstraints(const std::vector<Cage> &cages, const std::vector<Box> &boxes);
    void solve();
    void reportResult();
    void printStats();

private:
    void createVars();

    void createEncodingConstraints();
    void createRowConstraints();
    void createColumnConstraints();
    void createBoxConstraints();
    void createCageConstraints();
};
