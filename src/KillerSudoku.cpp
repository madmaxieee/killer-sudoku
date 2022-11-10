#include <algorithm>
#include <random>
#include <cstdlib>
#include <unordered_map>

#include "KillerSudoku.h"

KillerSudoku::KillerSudoku(int size)
{
    this->size = size;
    this->rng = std::default_random_engine{static_cast<unsigned int>(time(0))};

    this->initBoxes();
}

KillerSudoku::~KillerSudoku()
{
}

void KillerSudoku::newGame()
{
    this->generateNewSolution();
    this->generateCages();
}

void KillerSudoku::printSolution()
{
    for (int i = 0; i < size * size; i++)
    {
        for (int j = 0; j < size * size; j++)
        {
            // pad single digit numbers with a space
            if (this->solution[i][j] < 10)
            {
                std::cout << " ";
            }
            std::cout << this->solution[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void KillerSudoku::printCages()
{
    std::vector<std::vector<int>> cageGrid(size * size, std::vector<int>(size * size, 0));
    // fill cageGrid with cage sum
    for (int i = 0; i < this->cages.size(); i++)
    {
        for (int j = 0; j < this->cages[i].cells.size(); j++)
        {
            cageGrid[this->cages[i].cells[j].first][this->cages[i].cells[j].second] = this->cages[i].sum;
        }
    }

    for (int i = 0; i < size * size; i++)
    {
        for (int j = 0; j < size * size; j++)
        {
            // pad single digit numbers with a space
            if (cageGrid[i][j] < 10)
            {
                std::cout << " ";
            }
            std::cout << cageGrid[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<Box> &KillerSudoku::getBoxes()
{
    return this->boxes;
}

std::vector<std::vector<int>> &KillerSudoku::getSolution()
{
    return this->solution;
}

std::vector<Cage> &KillerSudoku::getCages()
{
    return this->cages;
}

void KillerSudoku::initBoxes()
{
    this->boxes = std::vector<Box>();
    this->boxes.reserve(size * size * 3);
    for (int i = 0; i < size * size; i += size)
    {
        for (int j = 0; j < size * size; j += size)
        {
            Box box;
            for (int k = 0; k < size; k++)
            {
                for (int l = 0; l < size; l++)
                {
                    box.insert(std::make_pair(i + k, j + l));
                }
            }
            this->boxes.push_back(box);
        }
    }
    // generate rows and columns
    for (int i = 0; i < size * size; i++)
    {
        Box row;
        Box col;
        for (int j = 0; j < size * size; j++)
        {
            row.insert(std::make_pair(i, j));
            col.insert(std::make_pair(j, i));
        }
        this->boxes.push_back(row);
        this->boxes.push_back(col);
    }
}

void KillerSudoku::generateNewSolution()
{
    this->solution = std::vector<std::vector<int>>(size * size,
                                                   std::vector<int>(size * size, 0));

    // fill diagonal boxes
    for (int i = 0; i < size * size; i += size)
    {
        std::vector<int> nums;
        for (int j = 1; j <= size * size; j++)
        {
            nums.push_back(j);
        }
        std::shuffle(nums.begin(), nums.end(), this->rng);
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                this->solution[i + j][i + k] = nums[j * size + k];
            }
        }
    }

    this->fillCells(0, 0);
}

bool KillerSudoku::fillCells(int row, int col)
{
    if (row == size * size)
    {
        return true;
    }
    if (col == size * size)
    {
        return this->fillCells(row + 1, 0);
    }
    if (this->solution[row][col] != 0)
    {
        return this->fillCells(row, col + 1);
    }
    if (row / size == col / size)
    {
        return this->fillCells(row, col + 1);
    }

    std::vector<int> possibleNums;
    for (int i = 1; i <= size * size; i++)
    {
        possibleNums.push_back(i);
    }

    // loop through boxes, remove numbers that are already in the box
    for (int i = 0; i < this->boxes.size(); i++)
    {
        if (this->boxes[i].find(std::make_pair(row, col)) != this->boxes[i].end())
        {
            for (int j = 0; j < size * size; j++)
            {
                for (int k = 0; k < size * size; k++)
                {
                    if (this->boxes[i].find(std::make_pair(j, k)) != this->boxes[i].end())
                    {
                        possibleNums.erase(std::remove(possibleNums.begin(), possibleNums.end(), this->solution[j][k]),
                                           possibleNums.end());
                    }
                }
            }
        }
    }

    if (possibleNums.empty())
    {
        return false;
    }
    else
    {
        std::shuffle(possibleNums.begin(), possibleNums.end(), this->rng);
        for (int i = 0; i < possibleNums.size(); i++)
        {
            this->solution[row][col] = possibleNums[i];
            if (this->fillCells(row, col + 1))
            {
                return true;
            }
        }
        this->solution[row][col] = 0;
        return false;
    }
}

void KillerSudoku::generateCages()
{
    this->cages = std::vector<Cage>();
    std::vector<std::vector<bool>> covered = std::vector<std::vector<bool>>(size * size,
                                                                            std::vector<bool>(size * size, false));
    // randomly select a cell that is not covered from a set
    std::vector<Cell> uncoveredCells;
    for (int i = 0; i < size * size; i++)
    {
        for (int j = 0; j < size * size; j++)
        {
            uncoveredCells.push_back(std::make_pair(i, j));
        }
    }
    std::shuffle(uncoveredCells.begin(), uncoveredCells.end(), this->rng);

    for (int i = 0; i < uncoveredCells.size(); i++)
    {
        Cell cell = uncoveredCells[i];
        if (!covered[cell.first][cell.second])
        {
            Cage cage;
            cage.sum = this->solution[cell.first][cell.second];
            cage.cells.push_back(std::make_pair(cell.first, cell.second));
            covered[cell.first][cell.second] = true;
            this->generateCage(cage, covered);
            this->cages.push_back(cage);
        }
    }

    // merge single cell cages with other cages adjacent cages
    std::vector<std::vector<int>> cellToCageIndex = std::vector<std::vector<int>>(size * size,
                                                                                  std::vector<int>(size * size, -1));
    std::vector<int> singleCellCageIndex = std::vector<int>();
    for (int i = 0; i < this->cages.size(); i++)
    {
        if (this->cages[i].cells.size() == 1)
        {
            singleCellCageIndex.push_back(i);
        }
        for (int j = 0; j < this->cages[i].cells.size(); j++)
        {
            cellToCageIndex[this->cages[i].cells[j].first][this->cages[i].cells[j].second] = i;
        }
    }

    for (int i = 0; i < singleCellCageIndex.size(); i++)
    {
        int cageIndex = singleCellCageIndex[i];
        Cell cell = this->cages[cageIndex].cells[0];
        std::vector<int> adjacentCageIndex;
        if (cell.first > 0)
        {
            adjacentCageIndex.push_back(cellToCageIndex[cell.first - 1][cell.second]);
        }
        if (cell.first < size * size - 1)
        {
            adjacentCageIndex.push_back(cellToCageIndex[cell.first + 1][cell.second]);
        }
        if (cell.second > 0)
        {
            adjacentCageIndex.push_back(cellToCageIndex[cell.first][cell.second - 1]);
        }
        if (cell.second < size * size - 1)
        {
            adjacentCageIndex.push_back(cellToCageIndex[cell.first][cell.second + 1]);
        }
        std::shuffle(adjacentCageIndex.begin(), adjacentCageIndex.end(), this->rng);
        if (adjacentCageIndex.size() > 0)
        {
            int index = adjacentCageIndex[0];
            this->cages[index].cells.push_back(cell);
            this->cages[index].sum += this->cages[cageIndex].sum;
        }
    }

    // erase single cell cages
    this->cages.erase(std::remove_if(this->cages.begin(), this->cages.end(),
                                     [](Cage cage)
                                     { return cage.cells.size() == 1; }),
                      this->cages.end());
}

void KillerSudoku::generateCage(Cage &cage, std::vector<std::vector<bool>> &covered)
{
    // max size is a random number with normal distribution centered at size
    int maxSize = std::max(2, std::min(size * size, (int)std::round(std::normal_distribution<double>(size, 1)(this->rng))));
    // chose a random cell from the cage
    Cell cell;
    std::vector<int> directions = {0, 1, 2, 3};
    std::unordered_map<Cell, int, CellHash> explored;
    // copy cage cells to toExplore
    std::vector<Cell> toExplore = cage.cells;

    while (cage.cells.size() < maxSize && !toExplore.empty())
    {
        cell = toExplore[this->rng() % toExplore.size()];

        if (explored[cell] == 0x0F)
        {
            toExplore.erase(std::remove(toExplore.begin(), toExplore.end(), cell), toExplore.end());
            continue;
        }
        // shuffle four directions
        std::shuffle(directions.begin(), directions.end(), this->rng);
        // loop through directions, try to grow the cage
        for (int i = 0; i < directions.size(); i++)
        {
            int row = cell.first;
            int col = cell.second;
            switch (directions[i])
            {
            case 0:
                row--;
                break;
            case 1:
                row++;
                break;
            case 2:
                col--;
                break;
            case 3:
                col++;
                break;
            }

            auto it = explored.find(cell);
            if (it == explored.end())
            {
                explored.insert(std::make_pair(cell, 0));
            }
            else
            {
                it->second |= (1 << directions[i]);
            }

            Cell newCell = std::make_pair(row, col);

            if (row >= 0 && row < size * size && col >= 0 && col < size * size && !covered[row][col])
            {
                cage.cells.push_back(newCell);
                cage.sum += this->solution[row][col];
                covered[row][col] = true;

                if (cage.cells.size() >= maxSize)
                {
                    return;
                }
            }
        }
    }
}
