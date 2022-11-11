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
                    if (k + 1 < 10)
                    {
                        std::cout << " ";
                    }
                    std::cout << k + 1 << " ";
                }
            }
        }
        std::cout << std::endl;
    }
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
    createRowConstraints();
    createColumnConstraints();
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

void KSSolver::createRowConstraints()
{
    for (int i = 0; i < size * size; i++)
    {
        for (int k = 0; k < size * size; k++)
        {
            std::vector<Lit> lits;
            for (int j = 0; j < size * size; j++)
            {
                lits.push_back(Lit(variables[i][j][k]));
            }
            _solver.addSimplePBClause(lits, std::vector<int>(size * size, 1), 1);
        }
    }
}

void KSSolver::createColumnConstraints()
{
    for (int j = 0; j < size * size; j++)
    {
        for (int k = 0; k < size * size; k++)
        {
            std::vector<Lit> lits;
            for (int i = 0; i < size * size; i++)
            {
                lits.push_back(Lit(variables[i][j][k]));
            }
            _solver.addSimplePBClause(lits, std::vector<int>(size * size, 1), 1);
        }
    }
}

void KSSolver::createBoxConstraints()
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size * size; k++)
            {
                std::vector<Lit> lits;
                for (int ii = 0; ii < size; ii++)
                {
                    for (int jj = 0; jj < size; jj++)
                    {
                        lits.push_back(Lit(variables[i * size + ii][j * size + jj][k]));
                    }
                }
                _solver.addSimplePBClause(lits, std::vector<int>(size * size, 1), 1);
            }
        }
    }
}

void ::KSSolver::createCageConstraints()
{
    for (auto cage : cages)
    {
        std::vector<Lit> lits;
        std::vector<int> weights;
        for (auto cell : cage.cells)
        {
            for (int k = 0; k < size * size; k++)
            {
                lits.push_back(Lit(variables[cell.first][cell.second][k]));
                weights.push_back(k + 1);
            }
        }
        _solver.addSimplePBClause(lits, weights, cage.sum);
    }
}
