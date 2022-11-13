#include <iostream>
#include "KillerSudoku.h"
#include "KSSolver.h"

int main()
{
    int size = 3;
    KillerSudoku ks(size);
    ks.generateNewSolution();
    ks.printSolution();
    ks.generateCages();
    ks.printCages();

    KSSolver solver(size);
    // time the code
    auto start = std::chrono::high_resolution_clock::now();

    solver.createConstraints(ks.getCages());
    solver.printStats();
    solver.solve();
    solver.reportResult();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    std::cout << std::endl;

    start = std::chrono::high_resolution_clock::now();
    solver.bruteForceSolve();
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    return 0;
}
