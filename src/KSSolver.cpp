#include "KSSolver.h"

KSSolver::KSSolver(int size)
{
    this->size = size;
    createVars();
}

KSSolver::~KSSolver()
{
}

void KSSolver::solve(const std::vector<Cage> &cages, bool verbose)
{
    this->cages = cages;
    if (verbose)
    {
        std::cout << "Solving Killer Sudoku with SAT solver..." << std::endl;
    }
    createConstraints(cages);
    _solver.solve();
    if (verbose)
    {
        reportResult();
    }
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

void KSSolver::printStats()
{
    _solver.printStats();
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

void KSSolver::createConstraints(const std::vector<Cage> &cages)
{
    this->cages = cages;
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

void KSSolver::createCageConstraints()
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

std::vector<std::vector<int>> KSSolver::getSolution()
{
    std::vector<std::vector<int>> solution(size * size, std::vector<int>(size * size, 0));
    for (int i = 0; i < size * size; i++)
    {
        for (int j = 0; j < size * size; j++)
        {
            for (int k = 0; k < size * size; k++)
            {
                if (_solver.getValue(variables[i][j][k]) == 1)
                {
                    solution[i][j] = k + 1;
                }
            }
        }
    }
    return solution;
}

void KSSolver::bruteForceSolve(const std::vector<Cage> &cages, bool verbose)
{
    this->cages = cages;
    if (verbose)
    {
        std::cout << "Solving Killer Sudoku with brute force..." << std::endl;
    }
    solution = std::vector<std::vector<int>>(size * size, std::vector<int>(size * size, 0));

    cell2cage.resize(size * size);
    for (int i = 0; i < size * size; i++)
    {
        cell2cage[i].resize(size * size);
    }
    for (auto cage : cages)
    {
        for (auto cell : cage.cells)
        {
            cell2cage[cell.first][cell.second] = cage;
        }
    }

    bool success = bruteForce();

    if (verbose && success)
    {
        for (int i = 0; i < size * size; i++)
        {
            for (int j = 0; j < size * size; j++)
            {
                if (solution[i][j] < 10)
                {
                    std::cout << " ";
                }
                std::cout << solution[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

std::pair<int, int> KSSolver::getFirstEmptyCell()
{
    for (int i = 0; i < size * size; i++)
    {
        for (int j = 0; j < size * size; j++)
        {
            if (solution[i][j] == 0)
            {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

bool KSSolver::bruteForce()
{
    std::pair<int, int> emptyCell = getFirstEmptyCell();
    int row = emptyCell.first;
    int col = emptyCell.second;

    if (row == -1 && col == -1)
    {
        return true;
    }

    if (row == size * size)
    {
        return true;
    }

    Cage cage = cell2cage[row][col];
    int emptyCells = cage.cells.size();
    int diff = cage.sum;
    for (auto cell : cage.cells)
    {
        if (solution[cell.first][cell.second] != 0)
        {
            diff -= solution[cell.first][cell.second];
            emptyCells--;
        }
    }

    std::unordered_set<int> candidates;
    // calculate min and max
    int max = std::min(diff - 1 * (emptyCells - 1), 9);
    int min = std::max(diff - size * size * (emptyCells - 1), 1);

    for (int i = min; i <= max; i++)
    {
        candidates.insert(i);
    }

    // remove candidates from row and column
    for (int i = 0; i < size * size; i++)
    {
        candidates.erase(solution[row][i]);
        candidates.erase(solution[i][col]);
    }

    // remove candidates from box
    int boxRow = row / size;
    int boxCol = col / size;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            candidates.erase(solution[boxRow * size + i][boxCol * size + j]);
        }
    }

    // try to solve recursively
    for (int candidate : candidates)
    {
        solution[row][col] = candidate;

        if (bruteForce())
        {
            return true;
        }
    }

    solution[row][col] = 0;
    return false;
}

std::vector<std::vector<int>> KSSolver::getBruteForceSolution()
{
    return solution;
}
