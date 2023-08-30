#include <chrono>
#include <cstdio>
#include <cstddef>
#include <deque>
#include <array>
#include <bitset>
#include <memory>
#include <random>

#include "rbitset.hpp"

using RB::RuntimeBitset;
constexpr size_t warmups = 2, rounds = 4, rows = 1000, cols = 1000;


ushort prime() {
    static constexpr std::array<ushort, 100> primes =
    {
        4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493,
        4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583,
        4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657,
        4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751,
        4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831,
        4861, 4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937,
        4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003,
        5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087,
        5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179,
        5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279
    };

    static std::mt19937 mt(std::time(nullptr));
    static std::uniform_int_distribution<ushort> getRandom;

    return primes[getRandom(mt) % 100];
}

std::unique_ptr<RuntimeBitset<>[]> bitsetGridGen(const size_t rows, const size_t cols) {
    auto grid = std::make_unique<RuntimeBitset<>[]>(rows);

    for (size_t i = 0; i < rows; i++) {
        grid[i] = RuntimeBitset<>(cols, false);
    }

    return grid;
}

template <typename GridType>
size_t benchmarkGrid(GridType& grid) {

    // Warmup:
    for (size_t r = 0; r < warmups; r++) {
        for (size_t w = 0, i = 0; i < rows and w < 512; w++, i++)
            for (size_t j = 0; j < cols and w < 512; w++, j++)
                grid[i][j] = true;

        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                grid[i][j] = (i % 2 != j % 2) ^ (bool) grid[(j*prime()+prime())%rows][(i*prime()+prime())%cols];
                grid[(i*prime()-prime())%rows][(i*prime()-prime())%cols] = (bool) grid[(i*j)%rows][(i*j)%cols] ^ !(bool) grid[i][j];
            }
        }
    }

    // Measurement:
    std::chrono::time_point tbegin = std::chrono::high_resolution_clock::now();

    for (size_t w = 0, i = 0; i < rows and w < 512; w++, i++)
        for (size_t j = 0; j < cols and w < 512; w++, j++)
            grid[i][j] = true;

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            grid[i][j] = (i % 2 != j % 2) ^ (bool) grid[(j*prime()+prime())%rows][(i*prime()+prime())%cols];
            grid[(i*prime()-prime())%rows][(i*prime()-prime())%cols] = (bool) grid[(i*j)%rows][(i*j)%cols] ^ !(bool) grid[i][j];
        }
    }

    std::chrono::time_point tend = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(tend - tbegin).count();
}

template <typename GridType>
void printGrid(GridType& grid) {
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++)
            printf("%hhx", grid[i][j]);
        printf("\n");
    }
}

double getDeltaPercent(size_t n, size_t d) {
    if (n == 0 or d == 0)
        return 0;
    else
        return (( (double) n / (double) d ) - 1) * 100;
}

int main() {
    printf("Running %lu rounds with %lu rows and %lu cols.\n", rounds, rows, cols);

    std::unique_ptr<RuntimeBitset<>[]> runtimeBitsetGrid = bitsetGridGen(rows, cols);
    std::array<std::bitset<cols>, rows> compileTimeBitsetGrid;
    std::deque<std::deque<bool>> dequeBoolsGrid(rows, std::deque<bool>(cols, false));
    std::array<std::array<bool, cols>, rows> arrayBoolsGrid;
    bool rawMatrixBoolsGrid[rows][cols];

    size_t usRuntimeBitset = 0;
    size_t usCompileTimeBitset = 0;
    size_t usDequeBools = 0;
    size_t usArrayBools = 0;
    size_t usRawMatrixBoolsGrid = 0;

    for (size_t i = 0; i < rounds; i++) {
        printf("\rRound: %lu", i+1);
        fflush(stdout);
        usRuntimeBitset += benchmarkGrid(runtimeBitsetGrid);
        usCompileTimeBitset += benchmarkGrid(compileTimeBitsetGrid);
        usDequeBools += benchmarkGrid(dequeBoolsGrid);
        usArrayBools += benchmarkGrid(arrayBoolsGrid);
        usRawMatrixBoolsGrid += benchmarkGrid(rawMatrixBoolsGrid);
    }

    usRuntimeBitset /= rounds;
    usCompileTimeBitset /= rounds;
    usDequeBools /= rounds;
    usArrayBools /= rounds;
    usRawMatrixBoolsGrid /= rounds;

    printf("\rRound: Done\n------------------------------------------------------------------\n");
    printf("RuntimeBitset<>(cols)[rows]              | %lu us \t| Baseline\n", usRuntimeBitset);
    printf("std::array<std::bitset<cols>, rows>      | %lu us \t| %+.1f%%\n", usCompileTimeBitset, getDeltaPercent(usCompileTimeBitset, usRuntimeBitset));
    printf("std::deque<std::deque<bool>>             | %lu us \t| %+.1f%%\n", usDequeBools, getDeltaPercent(usDequeBools, usRuntimeBitset));
    printf("std::array<std::array<bool, cols>, rows> | %lu us \t| %+.1f%%\n", usArrayBools, getDeltaPercent(usArrayBools, usRuntimeBitset));
    printf("bool[rows][cols]                         | %lu us \t| %+.1f%%\n", usRawMatrixBoolsGrid, getDeltaPercent(usRawMatrixBoolsGrid, usRuntimeBitset));
    printf("------------------------------------------------------------------\n");

    return 0;
}