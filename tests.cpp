#include <bits/chrono.h>
#include <cstdio>
#include <cstddef>
#include <memory>

#include <array>
#include <bitset>
#include <deque>

#include <chrono>

#include "rbitset.hpp"

using RB::RuntimeBitset;
constexpr size_t rows = 2000, cols = 2000;


std::unique_ptr<RuntimeBitset<>[]> bitsetGridGen(const size_t rows, const size_t cols) {
    auto grid = std::make_unique<RuntimeBitset<>[]>(rows);

    for (size_t i = 0; i < rows; i++) {
        grid[i] = RuntimeBitset<>(cols, false);
    }

    return grid;
}

size_t benchmarkRuntimeBitset() {
    std::chrono::time_point tbegin = std::chrono::high_resolution_clock::now();

    auto grid = bitsetGridGen(rows, cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            grid[i][j] = (i%2 == j%2);
            //printf("%hhx ", (bool) grid[i][j]);
        }
        //printf("\n");
    }
    //printf("Each bitset: %lu bools in %lu bytes.\n", grid[0].size(), grid[0].bytes());

    std::chrono::time_point tend = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbegin).count();
}

size_t benchmarkCompileTimeBitset() {
    std::chrono::time_point tbegin = std::chrono::high_resolution_clock::now();

    std::array<std::bitset<cols>, rows> grid;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            grid[i][j] = (i%2 == j%2);
            //printf("%hhx ", (bool) grid[i][j]);
        }
        //printf("\n");
    }

    std::chrono::time_point tend = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbegin).count();
}

size_t benchmarkDequeBools() {
    std::chrono::time_point tbegin = std::chrono::high_resolution_clock::now();

    std::deque<std::deque<bool>> grid(rows, std::deque<bool>(cols, false));

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            grid[i][j] = (i%2 == j%2);
            //printf("%hhx ", (bool) grid[i][j]);
        }
        //printf("\n");
    }

    std::chrono::time_point tend = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(tend - tbegin).count();
}

double getDeltaPercent(size_t n, size_t d) {
    if (n == 0 || d == 0)
        return 0;
    else
        return (( (double) n / (double) d ) - 1) * 100;
}

int main() {
    size_t msRuntimeBitset = benchmarkRuntimeBitset();
    size_t msCompileTimeBitset = benchmarkCompileTimeBitset();
    size_t msDequeBools = benchmarkDequeBools();

    printf("Running with %lu rows and %lu cols.\n", rows, cols);
    printf("------------------------------------------------------------\n");
    printf("| A | RuntimeBitset<>(cols)[rows] completed in: %lums.\n", msRuntimeBitset);
    printf("| B | std::array<std::bitset<cols>, rows> completed in: %lums.\n", msCompileTimeBitset);
    printf("| C | std::deque<std::deque<bool>> completed in: %lums.\n", msDequeBools);
    printf("------------------------------------------------------------\n");
    printf("A performs %+.1f%% than B.\n", getDeltaPercent(msRuntimeBitset, msCompileTimeBitset));
    printf("A performs %+.1f%% than C.\n", getDeltaPercent(msRuntimeBitset, msDequeBools));

    return 0;
}