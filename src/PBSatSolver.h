#pragma once

#include <vector>
#include "Solver.h"

class PBSatSolver
{
public:
    PBSatSolver(){};
    ~PBSatSolver(){};
    bool solve()
    {
        _solver.solve();
        return _solver.okay();
    };
    Var newVar()
    {
        return _solver.newVar();
    };
    void printStats()
    {
        _solver.printStats();
    };
    int getValue(Var var)
    {
        return _solver.modelValue(var) == l_True    ? 1
               : _solver.modelValue(var) == l_False ? 0
                                                    : -1;
    };

    void reportResult();
    void addClause(const std::vector<Lit> &clause);
    void addSimplePBClause(const std::vector<Lit> &clause, const std::vector<int> &weights, int sum);

private:
    Solver _solver;

    std::vector<std::vector<Lit>> HA3(Lit x, Lit y, Lit z, Lit carry, Lit sum);
    std::vector<std::vector<Lit>> HA2(Lit x, Lit y, Lit carry, Lit sum);
};
