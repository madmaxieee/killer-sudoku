#include "KSSolver.h"

KSSolver::KSSolver(int size)
{
    this->size = size;
    createVars();
}

KSSolver::~KSSolver()
{
}

void KSSolver::solve()
{
    _solver.solve();
}

void KSSolver::reportResult()
{
    _solver.reportResult();

    for (int i = 0; i < size * size; i++)
    {
        for (int j = 0; j < size * size; j++)
        {
            for (int k = 0; k < size * size; k++)
            {
                if (_solver.getValue(variables[i][j][k]) == 1)
                {
                    std::cout << k + 1 << " ";
                }
            }
        }
        std::cout << std::endl;
    }
}

void KSSolver::printClauses()
{
}

void KSSolver::createVars()
{
    variables.resize(size * size);
    for (int i = 0; i < size * size; i++)
    {
        variables[i].resize(size * size);
        for (int j = 0; j < size * size; j++)
        {
            variables[i][j].resize(size * size);
            for (int k = 0; k < size * size; k++)
            {
                variables[i][j][k] = _solver.newVar();
            }
        }
    }
}

void KSSolver::createConstraints(const std::vector<Cage> &cages, const std::vector<Box> &boxes)
{
    this->cages = cages;
    this->boxes = boxes;
    createEncodingConstraints();
    createBoxConstraints();
    createCageConstraints();
}

void KSSolver::createEncodingConstraints()
{
    for (int i = 0; i < size * size; i++)
    {
        for (int j = 0; j < size * size; j++)
        {
            std::vector<Lit> lits;
            for (int k = 0; k < size * size; k++)
            {
                lits.push_back(Lit(variables[i][j][k]));
            }
            _solver.addSimplePBClause(lits, std::vector<int>(size * size, 1), 1);
        }
    }
}

void ::KSSolver::createBoxConstraints()
{
}

void ::KSSolver::createCageConstraints()
{
}
