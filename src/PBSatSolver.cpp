#include <iostream>
#include <vector>
#include <queue>
#include "PBSatSolver.h"

void PBSatSolver::reportResult()
{
    if (!_solver.okay())
    {
        std::cout << "UNSAT" << std::endl;
        return;
    }
    _solver.printStats();
    std::cout << "SAT" << std::endl;
    std::cout << std::endl;
}

void PBSatSolver::addClause(const std::vector<Lit> &clause)
{
    vec<Lit> lits;
    for (auto lit : clause)
    {
        lits.push(lit);
    }
    _solver.addClause(lits);
}

// convert pseudo-boolean clause to normal clause via adder network
void PBSatSolver::addSimplePBClause(const std::vector<Lit> &clause, const std::vector<int> &weights, int sum)
{
    // calculate the number of bits needed to represent the sum
    int bucketCount = 0;
    int tmp = sum;
    while (tmp > 0)
    {
        tmp = tmp >> 1;
        bucketCount++;
    }

    // construct bit buckets
    std::vector<std::queue<Lit>> buckets(bucketCount);
    for (int i = 0; i < clause.size(); i++)
    {
        Lit lit = clause[i];
        int weight = weights[i];
        for (int j = 0; j < bucketCount; j++)
        {
            if (weight & (1 << j))
            {
                buckets[j].push(lit);
            }
        }
    }

    for (int i = 0; i < buckets.size(); i++)
    {
        while (buckets[i].size() >= 3)
        {
            Lit x = buckets[i].front();
            buckets[i].pop();
            Lit y = buckets[i].front();
            buckets[i].pop();
            Lit z = buckets[i].front();
            buckets[i].pop();
            Lit carry = Lit(_solver.newVar());
            Lit sum = Lit(_solver.newVar());
            std::vector<std::vector<Lit>> clauses = HA3(x, y, z, carry, sum);
            for (auto clause : clauses)
            {
                addClause(clause);
            }
            buckets[i].push(sum);
            // add bucket if needed
            if (i + 1 == buckets.size())
            {
                buckets.push_back(std::queue<Lit>());
            }
            buckets[i + 1].push(carry);
        }

        if (buckets[i].size() == 2)
        {
            Lit x = buckets[i].front();
            buckets[i].pop();
            Lit y = buckets[i].front();
            buckets[i].pop();
            Lit carry = Lit(_solver.newVar());
            Lit sum = Lit(_solver.newVar());
            std::vector<std::vector<Lit>> clauses = HA2(x, y, carry, sum);
            for (auto clause : clauses)
            {
                addClause(clause);
            }
            buckets[i].push(sum);
            // add bucket if needed
            if (i + 1 == buckets.size())
            {
                buckets.push_back(std::queue<Lit>());
            }
            buckets[i + 1].push(carry);
        }

        if (buckets[i].size() == 1)
        {
            addClause({(sum & (1 << i)) ? buckets[i].front() : ~buckets[i].front()});
        }

        if (buckets[i].size() == 0)
        {
            if (sum & (1 << i))
            {
                addClause({lit_Error});
            }
        }
    }
}

std::vector<std::vector<Lit>> PBSatSolver::HA3(Lit x, Lit y, Lit z, Lit carry, Lit sum)
{
    std::vector<std::vector<Lit>> clauses;
    clauses.push_back({z, ~carry, ~sum});
    clauses.push_back({x, y, ~carry});
    clauses.push_back({~z, carry, sum});
    clauses.push_back({~x, ~y, carry});
    clauses.push_back({x, y, z, ~sum});
    clauses.push_back({x, ~y, ~z, ~sum});
    clauses.push_back({~x, y, ~z, ~sum});
    clauses.push_back({~x, ~y, ~z, sum});
    clauses.push_back({~x, y, z, sum});
    clauses.push_back({x, ~y, z, sum});
    return clauses;
}

std::vector<std::vector<Lit>> PBSatSolver::HA2(Lit x, Lit y, Lit carry, Lit sum)
{
    std::vector<std::vector<Lit>> clauses;
    clauses.push_back({x, y, ~sum});
    clauses.push_back({~x, ~y, ~sum});
    clauses.push_back({x, ~carry});
    clauses.push_back({y, ~carry});
    clauses.push_back({~x, carry, sum});
    clauses.push_back({x, ~y, sum});
    return clauses;
}
