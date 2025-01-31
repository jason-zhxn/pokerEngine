#include "PokerGame.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

int main()
{
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }
    std::cin.rdbuf(inputFile.rdbuf());

    double tot = 0.0;

    for (int i = 0; i < 10000; i++) {
        inputFile.clear();
        inputFile.seekg(0, std::ios::beg);

        auto start = std::chrono::high_resolution_clock::now();

        PokerGame pokergame;
        pokergame.playGame();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

        std::cout << "Run " << i + 1 << ": " << duration_ns << " nanoseconds\n";
        tot += duration_ns;
    }

    std::cout << "Average execution time over 10000 runs: " << tot / 10000.0 << " nanoseconds\n";
    return 0;
}
