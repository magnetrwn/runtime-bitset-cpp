#include <bits/chrono.h>
#include <cstdio>
#include <cstddef>
#include <memory>

#include <array>
#include <bitset>

#include <chrono>

#include "rbitset.hpp"

using RB::RuntimeBitset;
constexpr size_t rows = 6000, cols = 4000;


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

int main() {
    size_t msRuntimeBitset, msCompileTimeBitset;
    msRuntimeBitset = benchmarkRuntimeBitset();
    msCompileTimeBitset = benchmarkCompileTimeBitset();

    printf("Running with %lu rows and %lu cols.\n", rows, cols);
    printf("RuntimeBitset<>(cols)[rows] completed in: %lums.\n", msRuntimeBitset);
    printf("std::array<std::bitset<cols>, rows> completed in: %lums.\n\n", msCompileTimeBitset);

    if (msRuntimeBitset != 0 and msCompileTimeBitset != 0) {
        double delta = (( (double) msRuntimeBitset / (double) msCompileTimeBitset ) - 1) * 100;
        printf("RuntimeBitset takes %+.1f%% time than the compile-time STL version.\n", delta);
    }

    return 0;
}