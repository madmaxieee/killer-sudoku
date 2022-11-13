#include <iostream>
#include "progressbar.hpp"
#include "KillerSudoku.h"
#include "KSSolver.h"

void test()
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

    solver.solve(ks.getCages());
    solver.reportResult();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    std::cout << std::endl;

    start = std::chrono::high_resolution_clock::now();
    solver.bruteForceSolve(ks.getCages());
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;
}

void benchmark(int size, int iterations)
{
    KillerSudoku ks(size);
    std::vector<double> satTimes = std::vector<double>(iterations);
    std::vector<double> bruteTimes = std::vector<double>(iterations);

    progressbar bar(iterations);
    for (int i = 0; i < iterations; i++)
    {
        bar.update();
        ks.generateNewSolution();
        ks.generateCages();

        KSSolver solver(size);
        // time the code
        auto start = std::chrono::high_resolution_clock::now();
        solver.solve(ks.getCages(), false);
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        satTimes[i] = elapsed.count();

        start = std::chrono::high_resolution_clock::now();
        solver.bruteForceSolve(ks.getCages(), false);
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        bruteTimes[i] = elapsed.count();
    }

    // report mean, standard deviation, and median
    double satSum = 0;
    double bruteSum = 0;
    for (int i = 0; i < iterations; i++)
    {
        satSum += satTimes[i];
        bruteSum += bruteTimes[i];
    }
    double satMean = satSum / iterations;
    double bruteMean = bruteSum / iterations;

    // calculate standard deviation
    double satDev = 0;
    double bruteDev = 0;
    for (int i = 0; i < iterations; i++)
    {
        satDev += (satTimes[i] - satMean) * (satTimes[i] - satMean);
        bruteDev += (bruteTimes[i] - bruteMean) * (bruteTimes[i] - bruteMean);
    }
    satDev = sqrt(satDev / iterations);
    bruteDev = sqrt(bruteDev / iterations);

    // calculate median
    std::sort(satTimes.begin(), satTimes.end());
    std::sort(bruteTimes.begin(), bruteTimes.end());
    double satMedian = satTimes[iterations / 2];
    double bruteMedian = bruteTimes[iterations / 2];

    std::cout << "\n\n ============ Benchmark Results ============" << std::endl;
    std::cout << "SAT-based solver mean: " << satMean << " s" << std::endl;
    std::cout << "SAT-based solver standard deviation: " << satDev << " s" << std::endl;
    std::cout << "SAT-based solver median: " << satMedian << " s" << std::endl;
    std::cout << "Brute force solver mean: " << bruteMean << " s" << std::endl;
    std::cout << "Brute force solver standard deviation: " << bruteDev << " s" << std::endl;
    std::cout << "Brute force solver median: " << bruteMedian << " s" << std::endl;
}

int main()
{
    benchmark(3, 100);
    return 0;
}
